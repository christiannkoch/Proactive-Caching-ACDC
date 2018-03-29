//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 
/* @file ReverseProxy.cc
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief Implements the reverse proxy module
 *
 * @section DESCRIPTION
 */
#include "CacheFactory.h"
#include "ReverseProxy.h"
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "NetworkInformation.h"
#include "SegmentRequest_m.h"
#include "VideoSegment_m.h"
#include "RequestHandler.h"
#include <vector>

using namespace omnetpp;
using namespace std;
Define_Module(ReverseProxy);

/*
 * @brief called upon initialization
 */
void ReverseProxy::initialize() {
    this->nwInfo = NetworkInformation::getInformation(this);
    this->settings = nwInfo->getSettings(par("proxyId"));
    this->cache = CacheFactory::createCache(settings->admissionStrategy,
            &settings->admissionParameters, settings->evictionStrategy,
            &settings->evictionParameters, settings->size,
            settings->storageAlterations, settings->storageAlterationStrategy,
            this);
    this->admission = cache->getAdmissionStrategy();
    this->eviction = cache->getEvictionStrategy();
    createClients();

    timeDifference = getParentModule()->par("recordGranularity");
    this->rqHandler = new RequestHandler();
    scheduleSelfMessages();
    hits.setName("hitsTotal");
    misses.setName("missesTotal");
    hitRate.setName("Hitrate");
    readOperations.setName("ReadOperations");
    writeOperations.setName("WriteOperations");
    avgDelay.setName("AverageDelay");
    timeToFirstSegment.setName("TimeToFirstSegment");
}

/*
 * @brief schedules selfmessages to periodically record data
 */
void ReverseProxy::scheduleSelfMessages() {
    scheduleAt(0.0, new cMessage("next Recording"));

    if (strcmp(settings->evictionStrategy.c_str(), "TTL") == 0
    /*|| strcmp(settings->evictionStrategy.c_str(), "ACARC") == 0*/)
        scheduleAt(0.0, new cMessage("periodic eviction Event"));

    if (strcmp(settings->admissionStrategy.c_str(), "NHIT") == 0)
        scheduleAt(0.0, new cMessage("periodic admission Event"));
}

/*
 * @brief defines the behaviour when a message arrives at the reverse proxy
 */
void ReverseProxy::handleMessage(cMessage *msg) {
    /*
     * if it is a next Recording event, we do our recordings and schedule another
     * next recording event
     */
    if (strcmp("next Recording", msg->getName()) == 0) {
        recordData();
        if (simTime() < nwInfo->getSimulationDuration())
            scheduleAt(simTime().dbl() + timeDifference, msg->dup());
        delete msg;
    }
    /*
     * if it is a periodic eviction event, we trigger the periodic eviction
     * events of the cache
     */
    else if (strcmp("periodic eviction Event", msg->getName()) == 0) {
        eviction->periodicEvents();
        if (simTime() < nwInfo->getSimulationDuration())
            scheduleAt(
                    simTime().dbl()
                            + std::stoi(settings->evictionParameters.at(0)),
                    msg->dup());
        delete msg;
    }
    /*
     * if it is a periodic admission event, we trigger the periodic admission
     * events of the cache
     */
    else if (strcmp("periodic admission Event", msg->getName()) == 0) {
        admission->periodicEvents();
        if (simTime() < nwInfo->getSimulationDuration()) {
            scheduleAt(simTime().dbl() + 3600.0, msg->dup());
        }
        delete msg;
    } else { // if it is not a periodic event
        const char* packetType = msg->getClassName();
        if (strcmp(packetType, "SegmentRequest") == 0) { // if it is a segment request
            SegmentRequest *rqst = check_and_cast<SegmentRequest *>(msg);
            /*
             * if the cache has the requested video segment stored, a hit is registered and
             * the video segment is served
             * if not, a cache miss is registered and
             * the request is added to the request queue of the request handler
             * and if a queue already existed, the request is deleted. if a queue did not exist
             * the request is forwarded to a proxy higher up in the hierarchy
             */
            if (eviction->contains(rqst)) {
                string keyBuilder = rqst->getVideoId()
                        + std::to_string(rqst->getSegmentId());
                registerCacheHit();
                if (rqst->getSegmentId() == 1) {
                    firstSegmentDelayVector.push_back(0.0);
                }
                //bubble("Hit");
                VideoSegment* pkg = eviction->retrieveSegment(rqst);
                pkg->setSeenAbove(true);
                sendMessage(pkg, rqst->getArrivalGate()->getIndex());

                delete rqst;
                if (settings->admissionStrategy == "MCD" && !settings->leaf)
                    eviction->deleteSegment(keyBuilder);
            } else {

                //bubble("Miss");
                string keyBuilder = rqst->getVideoId()
                        + std::to_string(rqst->getSegmentId());
                if (rqHandler->insertRequest(keyBuilder,
                        rqst->getArrivalGate()->getIndex())) {
                    registerCacheMiss();
                    sendMessage(rqst, 0);
                }

                else {
                    registerCacheHit();
                    if (rqst->getSegmentId() == 1) {
                        firstSegmentDelayVector.push_back(0.0);
                    }
                    delete rqst;
                }

            }
        }
        /*
         * if the message is a video segment, the admission strategy decides if it is stored
         * and the segment is served to all clients of the request queue
         */
        else {
            VideoSegment* package = check_and_cast<VideoSegment *>(msg);
            string keyBuilder = package->getVideoId()
                    + std::to_string(package->getSegmentId());
            vector<pair<int, double>*>* queue = rqHandler->getRequestList(
                    keyBuilder);
            if (admission->toBeCached(package))
                eviction->insertIntoCache(package->dup());
            for (vector<pair<int, double>*>::iterator iter = queue->begin();
                    iter != queue->end(); iter++) {
                VideoSegment *copy = (VideoSegment *) package->dup();
                copy->setSeenAbove(false);
                sendMessage(copy, (*iter)->first);
                delayVector.push_back(
                        omnetpp::simTime().dbl() - (*iter)->second);
                if (package->getSegmentId() == 1) {
                    firstSegmentDelayVector.push_back(
                            omnetpp::simTime().dbl() - (*iter)->second);
                }
            }
            rqHandler->deleteList(keyBuilder);
            delete package;
        }
    }
}

/*
 * @brief sends a message on a channel
 * @param msg the message to send
 * @param gate the gate on which we send the message
 */
void ReverseProxy::sendMessage(cMessage *msg, int gate) {
    const char* packetType = msg->getClassName();
    if (strcmp(packetType, "SegmentRequest") == 0) {
        SegmentRequest *rqst = check_and_cast<SegmentRequest *>(msg);
        send(rqst, "proxyOut", 0);
    } else {
        VideoSegment *package = check_and_cast<VideoSegment *>(msg);

        if (settings->leaf) {
            send(package, "clientOut", gate);
        } else {
            send(package, "proxyOut", gate);
        }
    }
}

void ReverseProxy::createClients() {
    if (settings->leaf) {

        std::cout << "started Client creation" << endl;
        const clock_t begin_time = clock();
        ClientCreator* cc = new ClientCreator(
                nwInfo->getClientIdsForReverseProxy(settings->name),
                getParentModule(), this);
        if (nwInfo->getClientIdsForReverseProxy(settings->name)->size()
                > 400000)
            cc->createClientsWithHubs();
        else
            cc->createClients();
        std::cout << "finished Client creation it took  "
                << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;
        delete cc;
    }

}

/*
 * @brief called at the end for garbage collection and recording purposes
 */
void ReverseProxy::finish() {
    eviction->clearCache();
    readOperations.record((double) cache->getReadOperations());
    writeOperations.record((double) cache->getWriteOperations());
    hitRate.record(cacheHit / (cacheHit + cacheMiss));
    recordScalar("Requests", cacheHitTotal + cacheMissTotal);
    recordScalar("Hit Rate", cacheHitTotal / (cacheHitTotal + cacheMissTotal));
    recordScalar("Write Operations", (double) cache->getWriteOperations());
    recordScalar("Read OPerations", (double) cache->getReadOperations());

    delete rqHandler;
    delete cache;
    //delete recordingEvent;
}

/*
 * @brief increments the cache hit value
 */
void ReverseProxy::registerCacheHit() {
    cacheHit++;
    cacheHitTotal++;
    delayVector.push_back(0.0);
}

/*
 * @brief increments the cache miss value
 */
void ReverseProxy::registerCacheMiss() {
    cacheMiss++;
    cacheMissTotal++;
}

/*
 * @brief records data for evaluation
 */
void ReverseProxy::recordData() {
    hits.record(cacheHitTotal);
    misses.record(cacheMissTotal);
    double divider = 1.0;
    if (cacheHit + cacheMiss > 0)
        divider = (cacheHit + cacheMiss);
    hitRate.record(cacheHit / divider);
    double delaySum = 0.0;
    for (auto i : delayVector) {
        delaySum = delaySum + i;
    }
    avgDelay.record(
            delaySum / ((delayVector.size() > 0) ? delayVector.size() : 1.0));
    double firstSegmentDelay = 0.0;
    for (auto i : firstSegmentDelayVector) {
        firstSegmentDelay = firstSegmentDelay + i;
    }
    timeToFirstSegment.record(
            firstSegmentDelay
                    / ((firstSegmentDelayVector.size() > 0) ?
                            firstSegmentDelayVector.size() : 1.0));
    readOperations.record((double) cache->getReadOperations());
    writeOperations.record((double) cache->getWriteOperations());
    eviction->resetRates();
    cacheHit = 0.0;
    cacheMiss = 0.0;
    delayVector.clear();
    firstSegmentDelayVector.clear();
}

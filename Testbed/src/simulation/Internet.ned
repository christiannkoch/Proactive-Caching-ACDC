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

package testbed.src.simulation;

network Internet
{
    parameters:
        //int numReverseProxys @prompt("Number of ReverseProxy's in the network") = default(1);
        //string clientCache = default("");
        //int proxyId = default(1);
        //int userId = default(1);
        //int clientId = default(1);
        //string admissionStrategy = default("");
        //string evictionStrategy = default("");
        //string youtubeCache = default("");
        //int sizeClient = default(100);
        //int sizeIsp = default(1000);
        //int sizeYoutube = default(1000);
        //int nrClients = default(-1);

        //string categoryID = default("-1");
        //string beginSimulation = default("-1");
        //string endSimulation = default("-1");
        //string onlyWithDailyViewcount = default("0");

        //string clientCacheParam = default("");
        string reverseProxyCacheParameters = default("admission=LCE;eviction=LFU;parameterList=10;");
        //string ytCacheParam = default("");
        string SetupfileName = default("");
		string name = default ("root");
		string password = default ("root");
    	double recordGranularity = default(3600.0);
    submodules:
        originServ: OriginServer;
    connections allowunconnected:
}

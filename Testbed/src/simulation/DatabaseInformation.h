/* @file DatabaseInformation.h
 * @author Johannes Pfannmüller, Christian Koch
 * @date
 * @version 1.0
 *
 * @brief a struct that holds all the database information
 *
 * @section DESCRIPTION
 */
#ifndef SRC_SIMULATION_DATABASEINFORMATION_H_
#define SRC_SIMULATION_DATABASEINFORMATION_H_

#include <string>

struct DatabseInformation_t {
    std::string type;
    std::string name;
    std::string table;
    std::string login;
    std::string password;
    std::string address;
    std::string category;
    int port;
    double startTime;
    double endTime;
};

#endif /* SRC_SIMULATION_DATABASEINFORMATION_H_ */

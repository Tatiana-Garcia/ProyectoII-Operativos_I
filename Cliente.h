

#ifndef PROYECTOII_OPERATIVOS_I_CLIENTE_H
#define PROYECTOII_OPERATIVOS_I_CLIENTE_H

#include <iostream>
using namespace std;

class Cliente {
public:
    enum class Operation {
        REVISION,
        PAGO,
        RETIRO
    };
    enum class Status {
        NUEVO,
        EN_COLA,
        EN_REVISION,
        EJECUTADO,
        TERMINADO
    };
    Cliente();
    Cliente(int id, Operation op, Status s, int time);

    // Getters
    Status getStatus();
    Operation getOperation();
    int getId();
    int getTime();
    bool getRevision();

    // Setters
    void setStatus(Status s);
    void setOperation(Operation op);
    void setId(int id);
    void setTime(int time);
    void setRevision(bool revision);

    // toString
    string operationToString(Operation op);
    string statusToString(Status s);

    // Print
    void printClient();
    void printStatus(string past_status);

private:
    int ID;
    Operation operation;
    Status status;
    int tiempo_llegada;
    bool revision = false; //flag de si ya paso por revision

};


#endif //PROYECTOII_OPERATIVOS_I_CLIENTE_H
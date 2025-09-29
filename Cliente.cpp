//
// Created by tatig on 9/16/2025.
//
#include "Cliente.h"
Cliente::Cliente(){}

Cliente::Cliente(int id, Operation op, Status s, int time) {
    this->ID = id;
    this->operation = op;
    this->status = s;
    this->tiempo_llegada = time;
}

int Cliente::getId() {
    return this->ID;
}

int Cliente::getTime() {
    return this->tiempo_llegada;
}

enum Cliente::Operation Cliente::getOperation() {
    return this->operation;
}

enum Cliente::Status Cliente::getStatus() {
    return this->status;
}

bool Cliente::getRevision() {
    return this->revision;
}

void Cliente::setId(int id) {
    this->ID = id;
}

void Cliente::setTime(int time) {
    this->tiempo_llegada = time;
}

void Cliente::setOperation(Operation op) {
    this->operation = op;
}

void Cliente::setStatus(Status status) {
    this->status = status;
}

void Cliente::setRevision(bool revision) {
    this->revision = revision;
}


string Cliente::operationToString(Operation op) {
    switch (op) {
        case Cliente::Operation::REVISION: return "Revision";
        case Cliente::Operation::PAGO:     return "Pago";
        case Cliente::Operation::RETIRO:   return "Retiro";
        default: return "Desconocido";
    }
}

string Cliente::statusToString(Status s) {
    switch (s) {
        case Cliente::Status::NUEVO:       return "Nuevo";
        case Cliente::Status::EN_COLA:     return "En cola";
        case Cliente::Status::EN_REVISION: return "En revision";
        case Cliente::Status::EJECUTADO:   return "Ejecutado";
        case Cliente::Status::TERMINADO:   return "Terminado";
        default: return "Desconocido";
    }
}

void Cliente::printClient() {
    cout << "Cliente: " << ID
         << ", Operacion: " << operationToString(operation)<< endl;
}

void Cliente::printStatus(string past_status) {
    cout << "Cliente " << ID << ": "<< past_status << " -> "<<statusToString(status) <<endl;
}


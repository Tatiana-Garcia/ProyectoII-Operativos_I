#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <random>
#include <chrono>
#include <vector>
#include <string>
#include "Cliente.h"
using namespace std;

// metodos de los hilos
void generarClientes();
void atenderCajero();
void manejarEspera();

int ID = 1;
queue<Cliente *> cola_principal;
queue<Cliente *> cola_espera;
mutex mtx;
int total_clientes = 0;
int clientes_atendidos = 0;
bool flag = false, wait = false;
int tiempo = 0;

// generador de numeros random
random_device rd;
mt19937 generator(rd());

int main()
{
    thread t1 = thread(generarClientes);
    thread t2 = thread(atenderCajero);
    thread t3 = thread(manejarEspera);

    t1.join();
    t2.join();
    t3.join();

    cout << "\nSe atendieron " << clientes_atendidos << " clientes" << endl
         << "Tiempo total de ejecucion: " << tiempo << " segundos." << endl;

    return 0;
}

void generarClientes()
{
    // Numeros aleatorios
    uniform_int_distribution<int> NumeroClientes(10, 20);
    uniform_int_distribution<int> Operacion(0, 2);
    uniform_int_distribution<int> tiempo_llegada(0, 5);

    int numero = NumeroClientes(generator); //
    total_clientes = numero;
    for (int i = 1; i <= numero; i++)
    {
        Cliente *c = new Cliente(
            ID,
            static_cast<Cliente::Operation>(Operacion(generator)),
            Cliente::Status::EN_COLA,
            tiempo_llegada(generator));
        ID++;

        mtx.lock();
        c->printClient();
        cola_principal.push(c);
        cout << "Cliente " + to_string(c->getId()) +
                    ": NUEVO -> EN_COLA "
             << endl;
        //(" + c->statusToString(c->getStatus()) + ")"
        mtx.unlock();

        int waiting_time = tiempo_llegada(generator);
        this_thread::sleep_for(chrono::seconds(waiting_time));
        tiempo += waiting_time;
    }
    flag = true;
}

void atenderCajero()
{
    while (true)
    {
        Cliente *c = nullptr;
        // bool flag2 = false;
        mtx.lock();
        if (!cola_principal.empty())
        {
            c = cola_principal.front();
            cola_principal.pop();
            c->setStatus(Cliente::Status::EJECUTADO);
            c->printStatus("En cola");
        }
        mtx.unlock();
        if (!c)
        {
            mtx.lock();
            if (flag && cola_principal.empty() && cola_espera.empty() &&
                clientes_atendidos == total_clientes)
            {
                mtx.unlock();
                break;
            }
            mtx.unlock();
            this_thread::sleep_for(1s);
            tiempo += 1;
            continue;
        }
        if (c->getOperation() == Cliente::Operation::PAGO ||
            c->getOperation() == Cliente::Operation::RETIRO || c->getRevision())
        {
            this_thread::sleep_for(2s);
            tiempo += 2;
            c->setStatus(Cliente::Status::TERMINADO);
            clientes_atendidos++;
            c->printStatus("Ejecutado");
        }
        else if (c->getOperation() == Cliente::Operation::REVISION)
        {
            c->setStatus(Cliente::Status::EN_REVISION);
            c->setRevision(true);
            c->printStatus("Ejecutado");
            mtx.lock();
            cola_espera.push(c);
            mtx.unlock();
        }
    }
}

void manejarEspera()
{
    while (true)
    {
        Cliente *c = nullptr;

        mtx.lock();
        if (!cola_espera.empty())
        {
            c = cola_espera.front();
            cola_espera.pop();
        }
        mtx.unlock();
        if (!c)
        {
            mtx.lock();
            if (flag && cola_principal.empty() && cola_espera.empty() &&
                clientes_atendidos == total_clientes)
            {
                mtx.unlock();
                break;
            }
            mtx.unlock();
            this_thread::sleep_for(1s);
            tiempo += 1;
        }

        if (c)
        {
            uniform_int_distribution<> dist(5, 10);
            int waiting_time = dist(generator);
            cout << "Cliente " << c->getId()
                 << ": En espera de revision (" << waiting_time << "s)" << endl;

            this_thread::sleep_for(chrono::seconds(waiting_time));
            tiempo += waiting_time;

            mtx.lock();
            c->setStatus(Cliente::Status::EN_COLA);
            cola_principal.push(c);
            mtx.unlock();

            c->printStatus("Revision");
        }
    }
}

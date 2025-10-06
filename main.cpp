#include <iostream>
#include <string>
#include <fstream>
#include <bitset>

using namespace std;

string DividirEnGrupos(string binario, int n);
string Codificar1(string Grupos);
string Decodificar1(string Grupos);
string Codificar2(string Grupos);
string Decodificar2(string Grupos);
string ConvertirAbinario(char Caracter);
string ConvertirATexto(string binario);
string TextoABinario(string texto);
string BinarioATexto(string binario);
bool VerificarClaveAdmin(string claveIngresada, int Semilla, int metodo);
void MenuAdministrador();
bool VerificarUsuario(string cedula, string clave, string& saldo);
void MenuUsuario();
void RegistrarUsuario();
void ActualizarSaldo(string cedula, string nuevoSaldo);

int main() {
    int opcion;

    do {
        cout << "\n===== CAJERO ELECTRONICO =====\n";
        cout << "1. Acceder como Administrador\n";
        cout << "2. Acceder como Usuario\n";
        cout << "3. Codificar/Decodificar archivo\n";
        cout << "4. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1: {
            string clave;
            int semilla;
            int metodo;

            cout << "Ingrese la clave de administrador: "; //Administrador123
            cin >> clave;
            cout << "Ingrese la semilla: "; //5
            cin >> semilla;
            cout << "Ingrese el metodo usado: "; //2
            cin >> metodo;

            if (VerificarClaveAdmin(clave, semilla, metodo)) {
                cout << "Acceso concedido como administrador.\n";
                MenuAdministrador();
            } else {
                cout << "Clave de administrador incorrecta.\n";
            }
            break;
        }
        case 2:
            MenuUsuario();
            break;
        case 3: {
            string nombreArchivo, texto = "", textoProcesado = "";
            int semilla, opcionOperacion, opcionMetodo;
            char caracter;

            cout << "Ingrese el nombre del archivo (sin el .txt): ";
            cin >> nombreArchivo;
            nombreArchivo += ".txt";

            cout << "Ingrese la semilla: ";
            cin >> semilla;

            cout << "Seleccione la operacion:\n1. Codificar\n2. Decodificar\nOpcion: ";
            cin >> opcionOperacion;

            cout << "Seleccione el metodo:\n1. Metodo 1\n2. Metodo 2\nOpcion: ";
            cin >> opcionMetodo;

            ifstream archivoEntrada(nombreArchivo);
            if (!archivoEntrada.is_open()) {
                cout << "No se pudo abrir el archivo: " << nombreArchivo << endl;
                break;
            }

            while (archivoEntrada.get(caracter)) {
                texto += caracter;
            }
            archivoEntrada.close();

            string binario, grupos, resultadoBinario, resultadoTexto;
            if (opcionOperacion == 1) {
                for (char c : texto) {
                    binario += ConvertirAbinario(c);
                }
                grupos = DividirEnGrupos(binario, semilla);

                textoProcesado = (opcionMetodo == 1) ? Codificar1(grupos) : Codificar2(grupos);

                ofstream salida("Archivo_Codificado.txt");
                if (salida.is_open()) {
                    salida << textoProcesado;
                    salida.close();
                    cout << "Archivo codificado correctamente.\n";
                } else {
                    cout << "Error al guardar archivo codificado.\n";
                }

            } else if (opcionOperacion == 2) {
                grupos = DividirEnGrupos(texto, semilla);
                resultadoBinario = (opcionMetodo == 1) ? Decodificar1(grupos) : Decodificar2(grupos);
                resultadoTexto = ConvertirATexto(resultadoBinario);

                ofstream salida("Archivo_Decodificado.txt");
                if (salida.is_open()) {
                    salida << resultadoTexto;
                    salida.close();
                    cout << "Archivo decodificado correctamente.\n";
                } else {
                    cout << "Error al guardar archivo decodificado.\n";
                }
            } else {
                cout << "Opcion invalida.\n";
            }
            break;
        }
        case 4:
            cout << "Saliendo del sistema...\n";
            break;
        default:
            cout << "Opcion no valida.\n";
        }
    } while (opcion != 4);

    return 0;
}

string DividirEnGrupos(string binario, int n) {
    string Grupos;
    string Grupo;

    for (int i = 0; i < binario.length(); i++) {
        Grupo += binario[i];
        if ((i + 1) % n == 0) {
            Grupos += Grupo + " ";
            Grupo.clear();
        }
    }

    if (!Grupo.empty()) {
        Grupos += Grupo;
    }

    return Grupos;
}

string TextoABinario(string texto) {
    string binario;
    for (char c : texto) {
        binario += ConvertirAbinario(c);
    }
    return binario;
}

string BinarioATexto(string binario) {
    return ConvertirATexto(binario);
}

string ConvertirAbinario(char Caracter) {
    bitset<8> binario(Caracter);
    return binario.to_string();
}

string ConvertirATexto(string binario) {
    string texto;
    for (size_t i = 0; i + 7 < binario.size(); i += 8) {
        string byte = binario.substr(i, 8);
        char caracter = static_cast<char>(bitset<8>(byte).to_ulong());
        texto += caracter;
    }
    return texto;
}

void MenuAdministrador() {
    int opcion;
    do {
        cout << "\n===== MENU ADMINISTRADOR =====\n";
        cout << "1. Registrar Usuario\n";
        cout << "2. Volver al Menu Principal\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            RegistrarUsuario();
        } else if (opcion != 2) {
            cout << "Opcion invalida. Intente de nuevo.\n";
        }
    } while (opcion != 2);
}

bool VerificarUsuario(string cedula, string clave, string& saldo) {
    ifstream archivo("usuarios.txt");
    if (!archivo.is_open()) {
        cout << "Error al abrir archivo de usuarios.\n";
        return false;
    }

    string cedulaCod = Codificar2(DividirEnGrupos(TextoABinario(cedula), 4));
    string claveCod = Codificar2(DividirEnGrupos(TextoABinario(clave), 4));
    string comaCod = Codificar2(DividirEnGrupos(TextoABinario(","), 4));
    string linea;

    while (getline(archivo, linea)) {
        size_t pos1 = linea.find(comaCod);
        if (pos1 == string::npos) continue;

        size_t pos2 = linea.find(comaCod, pos1 + comaCod.length());
        if (pos2 == string::npos) continue;

        if (linea.substr(0, pos1) == cedulaCod &&
            linea.substr(pos1 + comaCod.length(), pos2 - pos1 - comaCod.length()) == claveCod) {

            string saldoCod = linea.substr(pos2 + comaCod.length());
            saldo = BinarioATexto(Decodificar2(DividirEnGrupos(saldoCod, 4)));
            archivo.close();
            return true;
        }
    }

    archivo.close();
    return false;
}

bool VerificarClaveAdmin(string claveIngresada, int Semilla, int metodo) {
    string claveCodificada = "", gruposDecodificado;
    char caracter;

    ifstream archivoAdmin("sudo.txt");
    if (archivoAdmin.is_open()) {
        while (archivoAdmin.get(caracter)) {
            claveCodificada += caracter;
        }
        archivoAdmin.close();

        gruposDecodificado = DividirEnGrupos(claveCodificada, Semilla);
        string binario = (metodo == 1) ? Decodificar1(gruposDecodificado) : Decodificar2(gruposDecodificado);
        string decodificado = ConvertirATexto(binario);

        return decodificado == claveIngresada;
    } else {
        cout << "Error al abrir archivo de claves.\n";
        return false;
    }
}

void MenuUsuario() {
    string cedula, clave, saldo;
    cout << "\n===== ACCESO USUARIO =====\n";
    cout << "Ingrese su cedula: ";
    cin >> cedula;
    cout << "Ingrese su clave: ";
    cin >> clave;

    if (!VerificarUsuario(cedula, clave, saldo)) {
        cout << "Credenciales incorrectas.\n";
        return;
    }

    int saldoActual = stoi(saldo) - 1000;
    if (saldoActual < 0) {
        cout << "Saldo insuficiente para operar.\n";
        return;
    }

    saldo = to_string(saldoActual);
    ActualizarSaldo(cedula, saldo);
    cout << "Acceso concedido. Se cobraron 1000 COP.\n";

    int opcion;
    do {
        cout << "\n===== MENU USUARIO =====\n";
        cout << "1. Consultar Saldo\n";
        cout << "2. Retirar Dinero\n";
        cout << "3. Salir\n";
        cout << "Seleccione opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            cout << "\nSaldo actual: " << saldo << " COP\n";
        } else if (opcion == 2) {
            cout << "\nIngrese monto a retirar: ";
            int monto;
            cin >> monto;

            if (monto <= 0) {
                cout << "Monto no valido.\n";
            } else if (monto > stoi(saldo)) {
                cout << "Saldo insuficiente.\n";
            } else {
                saldoActual = stoi(saldo) - monto;
                saldo = to_string(saldoActual);
                ActualizarSaldo(cedula, saldo);
                cout << "Retiro exitoso. Nuevo saldo: " << saldo << " COP\n";
            }
        } else if (opcion != 3) {
            cout << "Opcion no valida.\n";
        }
    } while (opcion != 3);
}

void RegistrarUsuario() {
    string cedula, clave, saldo;
    cout << "Ingrese la cedula del usuario: ";
    cin >> cedula;
    cout << "Ingrese la clave del usuario: ";
    cin >> clave;
    cout << "Ingrese el saldo inicial (COP): ";
    cin >> saldo;

    string cedulaCod = Codificar2(DividirEnGrupos(TextoABinario(cedula), 4));
    string claveCod = Codificar2(DividirEnGrupos(TextoABinario(clave), 4));
    string saldoCod = Codificar2(DividirEnGrupos(TextoABinario(saldo), 4));
    string comaCod = Codificar2(DividirEnGrupos(TextoABinario(","), 4));

    ofstream archivo("usuarios.txt", ios::app);
    if (archivo.is_open()) {
        archivo << cedulaCod << comaCod << claveCod << comaCod << saldoCod << endl;
        archivo.close();
        cout << "Usuario registrado correctamente.\n";
    } else {
        cout << "Error al registrar usuario.\n";
    }
}

void ActualizarSaldo(string cedula, string nuevoSaldo) {
    ifstream entrada("usuarios.txt");
    if (!entrada.is_open()) {
        cout << "Error al abrir archivo de usuarios.\n";
        return;
    }

    string cedulaCod = Codificar2(DividirEnGrupos(TextoABinario(cedula), 4));
    string saldoCod = Codificar2(DividirEnGrupos(TextoABinario(nuevoSaldo), 4));
    string comaCod = Codificar2(DividirEnGrupos(TextoABinario(","), 4));
    string contenido, linea;

    while (getline(entrada, linea)) {
        size_t pos = linea.find(comaCod);
        if (pos == string::npos) {
            contenido += linea + "\n";
            continue;
        }

        if (linea.substr(0, pos) == cedulaCod) {
            size_t pos2 = linea.find(comaCod, pos + comaCod.length());
            if (pos2 != string::npos) {
                contenido += linea.substr(0, pos2 + comaCod.length()) + saldoCod + "\n";
                continue;
            }
        }
        contenido += linea + "\n";
    }

    entrada.close();

    ofstream salida("usuarios.txt");
    if (salida.is_open()) {
        salida << contenido;
        salida.close();
    } else {
        cout << "Error al guardar cambios.\n";
    }
}

string Codificar1(string Grupos) {
    string Codificado;
    string Grupo, GrupoA;
    int Contar0 = 0, Contar1 = 0;
    bool PrimerGrupo = true;

    for (int i = 0; i < Grupos.length(); i++) {
        if (Grupos[i] != ' ') {
            Grupo += Grupos[i];
        } else {
            if (PrimerGrupo) {
                for (char C : Grupo) {
                    Codificado += (C == '0') ? '1' : '0';
                }
                PrimerGrupo = false;
            } else {
                for (char C : GrupoA) {
                    if (C == '0') Contar0++;
                    else if (C == '1') Contar1++;
                }

                if (Contar0 == Contar1) {
                    for (char C : Grupo) {
                        Codificado += (C == '0') ? '1' : '0';
                    }
                } else if (Contar0 > Contar1) {
                    for (int i = 0; i < Grupo.length(); i++) {
                        if (i % 2 == 1) {
                            Codificado += (Grupo[i] == '0') ? '1' : '0';
                        } else {
                            Codificado += Grupo[i];
                        }
                    }
                } else {
                    for (int i = 0; i < Grupo.length(); i++) {
                        if (i % 3 == 2) {
                            Codificado += (Grupo[i] == '0') ? '1' : '0';
                        } else {
                            Codificado += Grupo[i];
                        }
                    }
                }
            }
            GrupoA = Grupo;
            Grupo.clear();
            Contar0 = 0;
            Contar1 = 0;
        }
    }

    if (!Grupo.empty()) {
        if (PrimerGrupo) {
            for (char C : Grupo) {
                Codificado += (C == '0') ? '1' : '0';
            }
        } else {
            for (char C : GrupoA) {
                if (C == '0') Contar0++;
                else if (C == '1') Contar1++;
            }

            if (Contar0 == Contar1) {
                for (char C : Grupo) {
                    Codificado += (C == '0') ? '1' : '0';
                }
            } else if (Contar0 > Contar1) {
                for (int i = 0; i < Grupo.length(); i++) {
                    if (i % 2 == 1) {
                        Codificado += (Grupo[i] == '0') ? '1' : '0';
                    } else {
                        Codificado += Grupo[i];
                    }
                }
            } else {
                for (int i = 0; i < Grupo.length(); i++) {
                    if (i % 3 == 2) {
                        Codificado += (Grupo[i] == '0') ? '1' : '0';
                    } else {
                        Codificado += Grupo[i];
                    }
                }
            }
        }
    }
    return Codificado;
}

string Decodificar1(string Grupos) {
    string Decodificado;
    string Grupo, GrupoAnterior;
    int Contar0 = 0, Contar1 = 0;
    bool PrimerGrupo = true;

    for (int i = 0; i < Grupos.length(); i++) {
        if (Grupos[i] != ' ') {
            Grupo += Grupos[i];
        } else {
            if (PrimerGrupo) {
                for (char C : Grupo) {
                    Decodificado += (C == '0') ? '1' : '0';
                }
                GrupoAnterior = Decodificado.substr(0, Grupo.length());
                PrimerGrupo = false;
            } else {
                Contar0 = 0;
                Contar1 = 0;
                for (char C : GrupoAnterior) {
                    if (C == '0') Contar0++;
                    else Contar1++;
                }

                string GrupoDecodificado;
                if (Contar0 == Contar1) {
                    for (char C : Grupo) {
                        GrupoDecodificado += (C == '0') ? '1' : '0';
                    }
                } else if (Contar0 > Contar1) {
                    for (int j = 0; j < Grupo.length(); j++) {
                        if (j % 2 == 1) {
                            GrupoDecodificado += (Grupo[j] == '0') ? '1' : '0';
                        } else {
                            GrupoDecodificado += Grupo[j];
                        }
                    }
                } else {
                    for (int j = 0; j < Grupo.length(); j++) {
                        if (j % 3 == 2) {
                            GrupoDecodificado += (Grupo[j] == '0') ? '1' : '0';
                        } else {
                            GrupoDecodificado += Grupo[j];
                        }
                    }
                }
                Decodificado += GrupoDecodificado;
                GrupoAnterior = GrupoDecodificado;
            }
            Grupo.clear();
        }
    }

    if (!Grupo.empty()) {
        if (PrimerGrupo) {
            for (char C : Grupo) {
                Decodificado += (C == '0') ? '1' : '0';
            }
        } else {
            Contar0 = 0;
            Contar1 = 0;
            for (char C : GrupoAnterior) {
                if (C == '0') Contar0++;
                else Contar1++;
            }

            string GrupoDecodificado;
            if (Contar0 == Contar1) {
                for (char C : Grupo) {
                    GrupoDecodificado += (C == '0') ? '1' : '0';
                }
            } else if (Contar0 > Contar1) {
                for (int j = 0; j < Grupo.length(); j++) {
                    if (j % 2 == 1) {
                        GrupoDecodificado += (Grupo[j] == '0') ? '1' : '0';
                    } else {
                        GrupoDecodificado += Grupo[j];
                    }
                }
            } else {
                for (int j = 0; j < Grupo.length(); j++) {
                    if (j % 3 == 2) {
                        GrupoDecodificado += (Grupo[j] == '0') ? '1' : '0';
                    } else {
                        GrupoDecodificado += Grupo[j];
                    }
                }
            }
            Decodificado += GrupoDecodificado;
        }
    }

    return Decodificado;
}

string Codificar2(string Grupos) {
    string Codificado;
    string Grupo;
    int Longitud = 0;

    for (int i = 0; i < Grupos.length(); i++) {
        if (Grupos[i] != ' ') {
            Grupo += Grupos[i];
        } else {
            Longitud = Grupo.length();
            Codificado += Grupo[Longitud - 1];
            for (int j = 0; j < Longitud - 1; j++) {
                Codificado += Grupo[j];
            }
            Grupo.clear();
        }
    }

    if (!Grupo.empty()) {
        Longitud = Grupo.length();
        Codificado += Grupo[Longitud - 1];
        for (int j = 0; j < Longitud - 1; j++) {
            Codificado += Grupo[j];
        }
        Grupo.clear();
    }

    return Codificado;
}

string Decodificar2(string Grupos) {
    string Decodificado;
    string Grupo;
    int Longitud = 0;
    char Caracter;

    for (int i = 0; i < Grupos.length(); i++) {
        if (Grupos[i] != ' ') {
            Grupo += Grupos[i];
        } else {
            Longitud = Grupo.length();
            Caracter = Grupo[0];
            for (int j = 1; j < Longitud; j++) {
                Grupo[j - 1] = Grupo[j];
            }
            Grupo[Longitud - 1] = Caracter;
            Decodificado += Grupo;
            Grupo.clear();
        }
    }

    if (!Grupo.empty()) {
        Longitud = Grupo.length();
        Caracter = Grupo[0];
        for (int j = 1; j < Longitud; j++) {
            Grupo[j - 1] = Grupo[j];
        }
        Grupo[Longitud - 1] = Caracter;
        Decodificado += Grupo;
        Grupo.clear();
    }

    return Decodificado;
}

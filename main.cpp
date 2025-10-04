#include <iostream>
#include <string>
#include <fstream>
#include <bitset>

using namespace std;

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

int main()
{
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
        case 1:{
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

            break;
        case 3:

            break;
        case 4:
            cout << "Saliendo del sistema...\n";
            break;
        default:
            cout << "Opcion no valida.\n";
            break;
        }


    } while(opcion != 4);
    return 0;
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

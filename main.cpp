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
        case 1:

            break;
        case 2:

            break;
        case 3:

            break;
        case 4:

            break;
        default:
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

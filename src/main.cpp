#include <iostream>
#include <string>
#include <cstdlib>

void printMenu() {
    std::cout << "===== Éditeur d'Images CLI =====" << std::endl;
    std::cout << "1. Panorama" << std::endl;
    std::cout << "2. Détection de visages" << std::endl;
    std::cout << "0. Quitter" << std::endl;
    std::cout << "Choix: ";
}

int main() {
    int choice;
    while (true) {
        printMenu();
        std::cin >> choice;
        if (!std::cin) break;
        switch (choice) {
        case 1: {
                std::string img1, img2;
                std::cout << "Chemin image 1: "; std::cin >> img1;
                std::cout << "Chemin image 2: "; std::cin >> img2;
                std::string cmd = "./panorama " + img1 + " " + img2;
                system(cmd.c_str());
                break;
        }
        case 2: {
                std::string cascade, img;
                std::cout << "Chemin cascade XML: "; std::cin >> cascade;
                std::cout << "Chemin image: "; std::cin >> img;
                std::string cmd2 = "./face_recognition " + cascade + " " + img;
                system(cmd2.c_str());
                break;
        }
        case 0:
            std::cout << "Au revoir !" << std::endl;
            return 0;
        default:
            std::cout << "Choix invalide." << std::endl;
        }
    }
    return 0;
}
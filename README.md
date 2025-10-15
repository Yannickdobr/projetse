#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "entreprise_allocation.h"

// Variables globales
SystemeEntreprise entreprise;
int window_width = 1200;
int window_height = 800;
int current_page = 0; // 0: Accueil, 1: Stock, 2: Employés, 3: Stratégies, 4: Résultats
int selected_strategy = -1;
int demandes_simulees = 0;
char message[200] = "Bienvenue dans le système d'allocation!";

// Couleurs
float color_blue[] = {0.2, 0.4, 0.8};
float color_green[] = {0.2, 0.7, 0.3};
float color_red[] = {0.9, 0.2, 0.2};
float color_orange[] = {1.0, 0.5, 0.0};
float color_purple[] = {0.6, 0.2, 0.8};
float color_gray[] = {0.7, 0.7, 0.7};
float color_light_gray[] = {0.9, 0.9, 0.9};

// Fonction pour dessiner du texte
void drawText(float x, float y, const char* text, void* font = GLUT_BITMAP_HELVETICA_12) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

// Fonction pour dessiner un bouton
int drawButton(float x, float y, float width, float height, const char* text, float* color) {
    // Dessiner le bouton
    glColor3fv(color);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Bordure noire
    glColor3f(0, 0, 0);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Texte centré
    glColor3f(0, 0, 0);
    int text_width = strlen(text) * 8;
    drawText(x + (width - text_width) / 2, y + height/2 - 4, text, GLUT_BITMAP_HELVETICA_12);
    
    return 1;
}

// Fonction pour dessiner une barre de progression
void drawProgressBar(float x, float y, float width, float height, float progress, float* color) {
    // Fond de la barre
    glColor3f(0.8, 0.8, 0.8);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Partie remplie
    glColor3fv(color);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width * progress, y);
    glVertex2f(x + width * progress, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Bordure
    glColor3f(0, 0, 0);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

// Page d'accueil
void drawHomePage() {
    // Titre
    glColor3f(0, 0, 0);
    drawText(400, 700, "SYSTEME D'ALLOCATION DES RESSOURCES", GLUT_BITMAP_TIMES_ROMAN_24);
    drawText(450, 670, "Entreprise de Vente Electronique", GLUT_BITMAP_HELVETICA_18);
    
    // Message de bienvenue
    glColor3f(0.3, 0.3, 0.3);
    drawText(450, 600, message, GLUT_BITMAP_HELVETICA_12);
    
    // Boutons de navigation
    drawButton(100, 500, 200, 50, "Voir le Stock", color_blue);
    drawButton(100, 420, 200, 50, "Voir les Employes", color_green);
    drawButton(100, 340, 200, 50, "Strategies d'Allocation", color_orange);
    drawButton(100, 260, 200, 50, "Simuler Demandes", color_purple);
    drawButton(100, 180, 200, 50, "Quitter", color_red);
    
    // Statistiques rapides
    glColor3f(0, 0, 0);
    drawText(500, 500, "=== STATISTIQUES RAPIDES ===", GLUT_BITMAP_HELVETICA_BOLD_12);
    
    char stats[100];
    sprintf(stats, "Produits en stock: %d", entreprise.nb_produits);
    drawText(500, 470, stats);
    
    sprintf(stats, "Employes: %d", entreprise.nb_employes);
    drawText(500, 450, stats);
    
    sprintf(stats, "Demandes en cours: %d", entreprise.nb_demandes);
    drawText(500, 430, stats);
    
    // Diagramme simple de répartition
    glColor3f(0, 0, 0);
    drawText(800, 500, "REPARTITION DES RESSOURCES", GLUT_BITMAP_HELVETICA_BOLD_12);
    
    // Dessiner des cercles pour représenter les produits
    float base_x = 800;
    float base_y = 400;
    
    for (int i = 0; i < entreprise.nb_produits && i < 5; i++) {
        float progress = (float)entreprise.produits[i].quantite_reservee / entreprise.produits[i].quantite_stock;
        
        // Cercle représentant le produit
        glColor3f(0.8, 0.8, 0.8);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(base_x + i * 70, base_y);
        for (int j = 0; j <= 360; j += 30) {
            float angle = j * 3.14159 / 180;
            glVertex2f(base_x + i * 70 + cos(angle) * 20, base_y + sin(angle) * 20);
        }
        glEnd();
        
        // Partie remplie
        glColor3f(0.2, 0.6, 1.0);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(base_x + i * 70, base_y);
        for (int j = 0; j <= 360 * progress; j += 30) {
            float angle = j * 3.14159 / 180;
            glVertex2f(base_x + i * 70 + cos(angle) * 20, base_y + sin(angle) * 20);
        }
        glEnd();
        
        // Bordure
        glColor3f(0, 0, 0);
        glLineWidth(1);
        glBegin(GL_LINE_LOOP);
        for (int j = 0; j <= 360; j += 30) {
            float angle = j * 3.14159 / 180;
            glVertex2f(base_x + i * 70 + cos(angle) * 20, base_y + sin(angle) * 20);
        }
        glEnd();
        
        // Nom du produit
        char prod_name[10];
        sprintf(prod_name, "P%d", i);
        drawText(base_x + i * 70 - 5, base_y - 30, prod_name);
    }
}

// Page d'affichage du stock
void drawStockPage() {
    glColor3f(0, 0, 0);
    drawText(500, 750, "STOCK DES PRODUITS ELECTRONIQUES", GLUT_BITMAP_TIMES_ROMAN_24);
    
    // En-tête du tableau
    glColor3f(0.3, 0.3, 0.6);
    drawText(100, 700, "Produit");
    drawText(300, 700, "Stock Total");
    drawText(450, 700, "Reserve");
    drawText(600, 700, "Disponible");
    drawText(750, 700, "Prix");
    drawText(900, 700, "Etat");
    
    // Ligne de séparation
    glColor3f(0, 0, 0);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(100, 690);
    glVertex2f(1000, 690);
    glEnd();
    
    // Données des produits
    for (int i = 0; i < entreprise.nb_produits; i++) {
        int y = 650 - i * 50;
        Produit *p = &entreprise.produits[i];
        int disponible = p->quantite_stock - p->quantite_reservee;
        float pourcentage = (float)p->quantite_reservee / p->quantite_stock;
        
        // Nom du produit
        glColor3f(0, 0, 0);
        drawText(100, y, p->nom);
        
        // Stock total
        char stock_str[20];
        sprintf(stock_str, "%d", p->quantite_stock);
        drawText(300, y, stock_str);
        
        // Quantité réservée
        sprintf(stock_str, "%d", p->quantite_reservee);
        drawText(450, y, stock_str);
        
        // Quantité disponible
        sprintf(stock_str, "%d", disponible);
        drawText(600, y, stock_str);
        
        // Prix
        char prix_str[20];
        sprintf(prix_str, "$%.2f", p->prix);
        drawText(750, y, prix_str);
        
        // Barre de progression
        drawProgressBar(850, y - 10, 150, 10, pourcentage, color_blue);
        
        // Pourcentage
        char percent_str[10];
        sprintf(percent_str, "%.0f%%", pourcentage * 100);
        drawText(1010, y, percent_str);
    }
    
    // Bouton retour
    drawButton(50, 50, 100, 30, "Retour", color_gray);
}

// Page d'affichage des employés
void drawEmployeesPage() {
    glColor3f(0, 0, 0);
    drawText(500, 750, "LISTE DES EMPLOYES", GLUT_BITMAP_TIMES_ROMAN_24);
    
    // En-tête du tableau
    glColor3f(0.3, 0.3, 0.6);
    drawText(100, 700, "ID");
    drawText(200, 700, "Nom");
    drawText(400, 700, "Role");
    drawText(600, 700, "Priorite");
    drawText(800, 700, "Statut");
    
    // Ligne de séparation
    glColor3f(0, 0, 0);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(100, 690);
    glVertex2f(1000, 690);
    glEnd();
    
    // Données des employés
    for (int i = 0; i < entreprise.nb_employes; i++) {
        int y = 650 - i * 50;
        Employe *e = &entreprise.employes[i];
        
        // ID
        char id_str[10];
        sprintf(id_str, "%d", e->id);
        drawText(100, y, id_str);
        
        // Nom
        drawText(200, y, e->nom);
        
        // Rôle
        drawText(400, y, e->role);
        
        // Priorité (étoiles)
        char stars[10];
        sprintf(stars, "");
        for (int j = 0; j < e->niveau_priorite; j++) {
            strcat(stars, "★");
        }
        drawText(600, y, stars);
        
        // Cercle de statut (vert = disponible, rouge = occupé)
        int demandes_attribuees = 0;
        for (int j = 0; j < entreprise.nb_demandes; j++) {
            if (entreprise.demandes[j].employe_attribue == i) {
                demandes_attribuees++;
            }
        }
        
        if (demandes_attribuees == 0) {
            glColor3f(0, 1, 0); // Vert = disponible
        } else if (demandes_attribuees < 3) {
            glColor3f(1, 1, 0); // Jaune = moyen chargement
        } else {
            glColor3f(1, 0, 0); // Rouge = très chargé
        }
        
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(850, y - 5);
        for (int j = 0; j <= 360; j += 30) {
            float angle = j * 3.14159 / 180;
            glVertex2f(850 + cos(angle) * 8, y - 5 + sin(angle) * 8);
        }
        glEnd();
        
        // Nombre de demandes
        char charge_str[20];
        sprintf(charge_str, "(%d demandes)", demandes_attribuees);
        glColor3f(0, 0, 0);
        drawText(870, y, charge_str);
    }
    
    // Bouton retour
    drawButton(50, 50, 100, 30, "Retour", color_gray);
}

// Page des stratégies d'allocation
void drawStrategiesPage() {
    glColor3f(0, 0, 0);
    drawText(500, 750, "STRATEGIES D'ALLOCATION", GLUT_BITMAP_TIMES_ROMAN_24);
    
    if (!demandes_simulees) {
        glColor3f(1, 0, 0);
        drawText(400, 700, "Veuillez d'abord simuler des demandes!", GLUT_BITMAP_HELVETICA_18);
        drawButton(450, 650, 300, 50, "Simuler des Demandes", color_purple);
    } else {
        // Description des stratégies
        glColor3f(0.3, 0.3, 0.3);
        drawText(100, 700, "Choisissez une strategie d'allocation:");
        
        // Boutons des stratégies
        drawButton(100, 650, 250, 60, "STATIQUE", color_blue);
        drawText(120, 630, "Produits fixes par employe", GLUT_BITMAP_HELVETICA_10);
        
        drawButton(400, 650, 250, 60, "DYNAMIQUE", color_green);
        drawText(420, 630, "Basé sur la charge de travail", GLUT_BITMAP_HELVETICA_10);
        
        drawButton(700, 650, 250, 60, "EQUITABLE", color_orange);
        drawText(720, 630, "Repartition egale", GLUT_BITMAP_HELVETICA_10);
        
        drawButton(1000, 650, 250, 60, "PRIORITAIRE", color_purple);
        drawText(1020, 630, "Clients VIP d'abord", GLUT_BITMAP_HELVETICA_10);
        
        // Visualisation des demandes actuelles
        glColor3f(0, 0, 0);
        drawText(100, 550, "DEMANDES EN ATTENTE:", GLUT_BITMAP_HELVETICA_BOLD_12);
        
        for (int i = 0; i < entreprise.nb_demandes; i++) {
            int y = 520 - i * 30;
            Demande *d = &entreprise.demandes[i];
            Produit *p = &entreprise.produits[d->id_produit];
            
            char demande_str[100];
            sprintf(demande_str, "Client %d: %d x %s (Priorite: %d)", 
                   d->id_client, d->quantite_demandee, p->nom, d->priorite_client);
            drawText(100, y, demande_str);
        }
    }
    
    // Bouton retour
    drawButton(50, 50, 100, 30, "Retour", color_gray);
}

// Page de résultats
void drawResultsPage() {
    glColor3f(0, 0, 0);
    drawText(500, 750, "RESULTATS DE L'ALLOCATION", GLUT_BITMAP_TIMES_ROMAN_24);
    
    // Afficher le message
    glColor3f(0.3, 0.3, 0.6);
    drawText(400, 700, message, GLUT_BITMAP_HELVETICA_12);
    
    // Tableau des résultats
    glColor3f(0.3, 0.3, 0.6);
    drawText(100, 650, "Demande");
    drawText(300, 650, "Client");
    drawText(450, 650, "Produit");
    drawText(650, 650, "Employe");
    drawText(850, 650, "Statut");
    
    for (int i = 0; i < entreprise.nb_demandes; i++) {
        int y = 620 - i * 40;
        Demande *d = &entreprise.demandes[i];
        Produit *p = &entreprise.produits[d->id_produit];
        Employe *e = &entreprise.employes[d->employe_attribue];
        
        char ligne[100];
        sprintf(ligne, "%d", d->id_demande);
        drawText(100, y, ligne);
        
        sprintf(ligne, "%d", d->id_client);
        drawText(300, y, ligne);
        
        drawText(450, y, p->nom);
        
        if (d->employe_attribue != -1) {
            drawText(650, y, e->nom);
        } else {
            drawText(650, y, "Non attribue");
        }
        
        if (d->statut == 1) {
            glColor3f(0, 0.7, 0);
            drawText(850, y, "APPROUVEE");
        } else if (d->statut == 2) {
            glColor3f(1, 0, 0);
            drawText(850, y, "REFUSEE");
        } else {
            glColor3f(0.5, 0.5, 0.5);
            drawText(850, y, "EN ATTENTE");
        }
        glColor3f(0, 0, 0);
    }
    
    // Statistiques finales
    int approuvees = 0, refusees = 0;
    for (int i = 0; i < entreprise.nb_demandes; i++) {
        if (entreprise.demandes[i].statut == 1) approuvees++;
        if (entreprise.demandes[i].statut == 2) refusees++;
    }
    
    glColor3f(0, 0, 0);
    drawText(100, 400, "=== STATISTIQUES FINALES ===", GLUT_BITMAP_HELVETICA_BOLD_12);
    
    char stats[100];
    sprintf(stats, "Demandes approuvees: %d", approuvees);
    drawText(100, 370, stats);
    
    sprintf(stats, "Demandes refusees: %d", refusees);
    drawText(100, 350, stats);
    
    sprintf(stats, "Taux de succes: %.1f%%", (float)approuvees / entreprise.nb_demandes * 100);
    drawText(100, 330, stats);
    
    // Boutons
    drawButton(50, 50, 100, 30, "Retour", color_gray);
    drawButton(170, 50, 150, 30, "Nouvelle Simulation", color_green);
}

// Fonction d'affichage principale
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    
    // Configuration de la vue orthographique
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, window_width, 0, window_height);
    glMatrixMode(GL_MODELVIEW);
    
    switch(current_page) {
        case 0: drawHomePage(); break;
        case 1: drawStockPage(); break;
        case 2: drawEmployeesPage(); break;
        case 3: drawStrategiesPage(); break;
        case 4: drawResultsPage(); break;
    }
    
    glutSwapBuffers();
}

// Gestion des clics de souris
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Convertir les coordonnées de la souris
        int mouse_x = x;
        int mouse_y = window_height - y;
        
        // Navigation entre pages
        if (current_page == 0) { // Page d'accueil
            if (mouse_x >= 100 && mouse_x <= 300) {
                if (mouse_y >= 500 && mouse_y <= 550) current_page = 1;
                else if (mouse_y >= 420 && mouse_y <= 470) current_page = 2;
                else if (mouse_y >= 340 && mouse_y <= 390) current_page = 3;
                else if (mouse_y >= 260 && mouse_y <= 310) {
                    simuler_scenarios(&entreprise);
                    demandes_simulees = 1;
                    strcpy(message, "5 demandes simulees avec succes!");
                    current_page = 3;
                }
                else if (mouse_y >= 180 && mouse_y <= 230) exit(0);
            }
        }
        // Bouton retour
        else if (mouse_x >= 50 && mouse_x <= 150 && mouse_y >= 50 && mouse_y <= 80) {
            current_page = 0;
        }
        // Stratégies
        else if (current_page == 3 && demandes_simulees) {
            if (mouse_x >= 100 && mouse_x <= 350 && mouse_y >= 650 && mouse_y <= 710) {
                // Stratégie statique
                for (int i = 0; i < entreprise.nb_demandes; i++) {
                    traiter_demande(&entreprise, i, 1);
                }
                strcpy(message, "Strategie STATIQUE appliquee!");
                current_page = 4;
            }
            else if (mouse_x >= 400 && mouse_x <= 650 && mouse_y >= 650 && mouse_y <= 710) {
                // Stratégie dynamique
                for (int i = 0; i < entreprise.nb_demandes; i++) {
                    traiter_demande(&entreprise, i, 2);
                }
                strcpy(message, "Strategie DYNAMIQUE appliquee!");
                current_page = 4;
            }
            else if (mouse_x >= 700 && mouse_x <= 950 && mouse_y >= 650 && mouse_y <= 710) {
                // Stratégie équitable
                for (int i = 0; i < entreprise.nb_demandes; i++) {
                    traiter_demande(&entreprise, i, 3);
                }
                strcpy(message, "Strategie EQUITABLE appliquee!");
                current_page = 4;
            }
            else if (mouse_x >= 1000 && mouse_x <= 1250 && mouse_y >= 650 && mouse_y <= 710) {
                // Stratégie prioritaire
                for (int i = 0; i < entreprise.nb_demandes; i++) {
                    traiter_demande(&entreprise, i, 4);
                }
                strcpy(message, "Strategie PRIORITAIRE appliquee!");
                current_page = 4;
            }
        }
        // Nouvelle simulation
        else if (current_page == 4 && mouse_x >= 170 && mouse_x <= 320 && mouse_y >= 50 && mouse_y <= 80) {
            initialiser_entreprise(&entreprise);
            demandes_simulees = 0;
            strcpy(message, "Systeme reinitialise!");
            current_page = 0;
        }
        
        glutPostRedisplay();
    }
}

// Redimensionnement de la fenêtre
void reshape(int w, int h) {
    window_width = w;
    window_height = h;
    glViewport(0, 0, w, h);
}

// Fonction principale
int main(int argc, char** argv) {
    // Initialisation du système
    initialiser_entreprise(&entreprise);
    
    // Initialisation de GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Systeme d'Allocation des Ressources - Entreprise");
    
    // Configuration OpenGL
    glClearColor(1.0, 1.0, 1.0, 1.0);
    
    // Enregistrement des callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    
    // Lancement de la boucle principale
    glutMainLoop();
    
    return 0;
}

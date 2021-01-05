/*
 -----------------------------------------------------------------------------------
 Laboratoire : L2b - Réseau social
 Fichier     : CFC.h
 Auteur(s)   : Dylan Canton, Mélodie Ohan, Yanik lange
 Date        : 24.10.2020

 But         :  Classe permettant la construction d'un graph orienté ainsi que
                d'un graphe dont les relations sont inversés.
            
 Remarque(s) : -

 Compilateur : MinGW-g++ 6.3.0
 -----------------------------------------------------------------------------------
 */
 
#ifndef SYMBOLGRAPH_H
#define SYMBOLGRAPH_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>

#include "Util.h"

template<typename GraphType>
class SymbolGraph {
    typedef GraphType Graph;

    std::map<std::string, int> symbolTable;

    std::vector<std::string> keys;
    
protected:
    Graph* g;
    Graph* r;        // ici se trouvera les relations "inversée" de g
                     // Ex: Si aRb, r contiendra bRa

public:

    ~SymbolGraph() {
        delete g;
        delete r;
    }

    //creation du SymbolGraph a partir du fichier movies.txt
    SymbolGraph(const std::string& filename, char delim = '/') {
        std::string line;

        std::ifstream s(filename);
        int cnt = 0;

        while (std::getline(s, line)) {
            auto names = split(line, delim);
            for (const auto& name : names) {
                if (!contains(name)) {
                    symbolTable[name] = cnt++;
                }
            }
        }
        s.close();

        keys.resize(cnt);
        for (const auto& pair : symbolTable)
            keys[pair.second] = pair.first;

        g  = new Graph(cnt);
        r  = new Graph(cnt);

        s.open(filename);
        while (std::getline(s, line)) {
            auto names = split(line, delim);

            int v = symbolTable[names[0]];
            
            for (size_t i = 1; i < names.size(); ++i) {
                int w = symbolTable[names[i]];
                r->addEdge(w, v);       // pour avoir le lien construit dans l'autre sens
                g->addEdge(v, w);
            }
        }
        s.close();
    }


    /**
     * @Author Dylan Canton, Mélodie Ohan, Yanik Lange
     * @Date 24/10/2020
     *
     * @brief       Construit un lien de v en direction de w pour le graph g
     *              et l'inverse pour le graph r.
     * @param name1 nom du 1er symbole
     * @param name1 nom du 2nd symbole
     * 
     */
    void addEdgeByName(const std::string& name1, const std::string& name2){
        
        int v = index(name1);
        int w = index(name2);
        
        g->addEdge(v,w);
        r->addEdge(w,v);    
    }

    //verifie la presence d'un symbole
    bool contains(const std::string& name) const {
        return symbolTable.find(name) != symbolTable.end();
    }

    //index du sommet correspondant au symbole
    int index(const std::string& name) const {
        return symbolTable.at(name);
    }

    //symbole correspondant au sommet
    std::string symbol(int idx) const {
        return keys[idx];
    }


    /**
     * @Author Dylan Canton, Mélodie Ohan, Yanik Lange
     * @Date 24/10/2020
     *
     * @brief           Fourni un vecteur de symboles adjacents au symbole name.
     * @param name      symbole dont les relations sont à dresser
     * @param reverse   Si est à true, le vecteur sera construit sur la base
     *                  du graph inversé.
     * @return          vecteur contenant les noms du cercle de relation
     * 
     */
    std::vector<std::string> adjacent(const std::string& name, bool reverse) const {

        if (!contains(name)) 
            return std::vector<std::string>(0);
        
        // si adjacent est appelée avec reverse à true, on récupèrera les relations
        // construite dans r plutôt que g
        auto& adjIdx = reverse ? r->adjacent(index(name)) : g->adjacent(index(name));
        
        int N = adjIdx.size();               
        std::vector<std::string> adjStr(N);  // contiendra le noms des adjacents

        std::transform(adjIdx.cbegin(), adjIdx.cend(), adjStr.begin(), [&](int i) { return this->symbol(i); });

        return adjStr;
    }

    const Graph& G() const {
        return *g;
    }
     /**
     * @Author Dylan Canton, Mélodie Ohan, Yanik Lange
     * @Date 24/10/2020
     *
     * @brief           Retourne le graph inversé 
     * @return          r, le graph inversé
     * 
     */
    const Graph& R() const{
        return *r;
    }

};

#endif    /* SYMBOLGRAPH_H */
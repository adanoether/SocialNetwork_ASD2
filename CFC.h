/*
 -----------------------------------------------------------------------------------
 Laboratoire : L2b - Réseau social
 Fichier     : CFC.h
 Auteur(s)   : Dylan Canton, Mélodie Ohan, Yanik lange
 Date        : 24.10.2020

 But         : Ce fichier contient les classes CFC et KosarajuSharirCFC permettant
               de récupèrer le cercle auquel un sommet donné appartient.

 Remarque(s) : - 

 Compilateur : MinGW-g++ 6.3.0
 -----------------------------------------------------------------------------------
 */
#ifndef CFC_H
#define CFC_H

#include <vector>
#include <algorithm>
#include "DFS.h"

// Classe mettant en oeuvre les composantes fortement connexes
template<typename GraphType>
class CFC {
protected:
    // numéro de la CFC à laquelle appartient chaque sommet (-1 si pas encore assigné)
    std::vector<int> id;
    
    // nombre de composantes connexes
    int count;

public:
    // v et w sont-ils connectés ?
    bool Connected(int v, int w) {
        return Id(v) == Id(w);
    }

    // à quelle CFC appartient v ?
    int Id(int v) {
        return id.at(v);
    }

    // combien y a-t-il de CC ?
    int Count() {
        return count;
    }
};

template<typename GraphType>
class KosarajuSharirCFC : public CFC<GraphType> {

    
public:


    /**
     * @Author Dylan Canton, Mélodie Ohan, Yanik Lange
     * @Date 27/09/2020
     *
     * @brief           Constructeur a partir du graphe g et du graphe inverse 
     *                  de g DFS itératif
     *
     * @param g         graphe orienté
     * @param gReverse  graphe orienté inversé
     */
    KosarajuSharirCFC(const GraphType& g, const GraphType& gReverse){
  
       // initialisation des graphs et du conteneur du resultat post ordre
        DFSIter<GraphType> dfsGraph(g);              
        DFSIter<GraphType> dfsRGraph(gReverse);      
        std::vector<int>   postOrder;           
        
        // premier parcours en profondeur
        dfsGraph.visitGraph([](int v){}, [&](int v){ postOrder.push_back(v);});
        
        // initialisation de id, on affecte à -1 pour reconnaître par la suite
        // les sommets pas liés à un cercle
        this->id.assign(postOrder.size(),-1); // init id à -1 
        this->count = 0;                      
        
        // Algorithm de Kosaraju Sharir
      
        // inverse le poste ordre
        std::reverse(postOrder.begin(), postOrder.end());
        
        // push dans id à la case du sommet à quel CFC il appartient
        for( int i = 0; i < postOrder.size();  ++i){
            if(this->Id(i) == -1)
                dfsRGraph.visit(postOrder[i], [](int w){}, [&](int w){ this->id.at(w) = this->count;});
                ++(this->count);    
        }
        
      }
    
    
    /**
     * @Author Dylan Canton, Mélodie Ohan, Yanik Lange
     * @Date 27/09/2020
     *
     * @brief       Retourne les id des sommets faisant parti du même cercle
     *
     * @param v     sommet dont on doit retourner le cercle
     * @return      vecteur contenant les id des sommets fortements connectés
     */
    std::vector<int> getCFC(int v){
        std::vector<int> cfc;
        
        // si la valeur est impossible
        if(v <0 || v >= this->Count())
            return cfc;
        
        // Si 
        for(int i = 0; i < this->Count(); ++i)
            if(this->Id(v) == this->Id(i) && v != i)
                cfc.push_back(i);
        
        return cfc;
    }
    
};

#endif
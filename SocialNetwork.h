/*
 -----------------------------------------------------------------------------------
 Laboratoire : L2b - Réseau social
 Fichier     : CFC.h
 Auteur(s)   : Dylan Canton, Mélodie Ohan, Yanik lange
 Date        : 24.10.2020

 But         : Mise à disposition d'une librairie permettant d'ajouter un abonnement, 
               récupérer la liste des abonnés ou des abonnements d'une personne.
               la librairie permet également de dresser le cercle de relationS 
               d'une personne. Cette librairie est basé sur SymbolGraph et
               KosarajuSharirCFC.
              

 Remarque(s) : - 

 Compilateur : MinGW-g++ 6.3.0
 -----------------------------------------------------------------------------------
 */
#ifndef ASD2_SocialNetwork_h
#define ASD2_SocialNetwork_h

#include <list>
#include <vector>
#include <exception>
#include "SymbolGraph.h"
#include "CFC.h"
#include "DiGraph.h"

template<typename GraphType>
class SocialNetwork : public SymbolGraph<GraphType> {

    typedef SymbolGraph<GraphType>       BASE;
    typedef KosarajuSharirCFC<GraphType> KSCFC;
    
    private:

public:

    SocialNetwork(const std::string& filename, char delim = '/')
    :BASE(filename,delim){}


    /**
     * @Author Dylan Canton, Mélodie Ohan, Yanik Lange
     * @Date 24/10/2020
     *
     * @brief      retourne les personnes auxquelles la personne "name" est abonnée
     * @param name personne dont les abonnements sont observés
     * @return     vecteur de la liste des prénoms dont name est abonné
     */
    std::vector<std::string> personsSubscribedBy(const std::string& name) const {
        return this->adjacent(name, false);
    }

    /**
     * @Author Dylan Canton, Mélodie Ohan, Yanik Lange
     * @Date 24/10/2020
     *
     * @brief   retourne les personnes qui sont abonnées "name"
     * @param   name personne suivie
     * @return  vecteur de la liste des prénoms des abonnés
     * 
     * Complexité en O(s), car parcours une liste contenant uniquement 
     * les folowers de name
     * 
     */
    std::vector<std::string> personsWhoFollows(const std::string& name) const {
        return this->adjacent(name, true);   
    }

    /**
     * @Author Dylan Canton, Mélodie Ohan, Yanik Lange
     * @Date 24/10/2020
     *
     * @brief                determine si subscriber suit personFollowed
     * @param subscriber     nom de l'abonné
     * @param personFollowed nom de la personne suivie
     * @return               vrai si subscriber suit personFollowed
     */
    bool isFollowing(const std::string& subscriber, const std::string& personFollowed) const {
       std::vector<std::string> subFollows = this->adjacent(subscriber,false);
       for(std::string followed : subFollows){
           if(followed == personFollowed)
                return true;
       }
       return false;
    }

    /**
     * @Author Dylan Canton, Mélodie Ohan, Yanik Lange
     * @Date 24/10/2020
     *
     * @brief                ajoute un nouvel abonnement au réseau social. 
     *                       L'abonnement représente subscriber suit personToFollow
     * @param subscriber     nom de l'abonné
     * @param personFollowed nom de la personne à suivre
     */
    void addSubscription(const std::string& subscriber, const std::string& personToFollow) {
       this->addEdgeByName(subscriber,personToFollow);
    }

     /**
     * @Author Dylan Canton, Mélodie Ohan, Yanik Lange
     * @Date 24/10/2020
     *
     * @brief      retourne les personnes faisant partie du cercle de relations
     *             de la personne "name"
     * @param name personne dont on doit établir le cercle
     * @return     vecteur contenant les noms du cercle de relation
     * 
     * Cette fonction se base sur la classe KosarajuSharirCFC afin de trouver 
     * le cercle de relations d'une personne
     */
    std::vector<std::string> relationCircle(const std::string& name) const {
        
        
        KSCFC cfc(this->G(), this->R());
        
        // on extrait l'id pour utiliser CFC
        int v = this->index(name);
        
        // Vecteur des ids et init du vecteurs des noms
        std::vector<int> indexes = cfc.getCFC(v);
        std::vector<std::string> relations(indexes.size());
        
        // Conversion de l'id au nom
        for(size_t i = 0; i < indexes.size(); ++i)
            relations[i] = this->symbol(indexes[i]);
        
        return relations;
    }
};

#endif

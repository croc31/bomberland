#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <vector>
using namespace std;
 
/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
typedef struct Node {
    int value;
    bool enemy = false;
    bool gateway = false;
    bool visited = false;
    list<Node*> *adj_list;
} Node;
 
vector<Node*> vetor;
 
void bloqueia(int enemyPosi){
    //marcando o vértice do inimigo
    vetor[enemyPosi]->visited = true;
    list<int> fila;
    fila.push_back(enemyPosi);
    // for(list<Node*>::iterator i = vetor[enemyPosi]->adj_list->begin(); 
    //     i != vetor[enemyPosi]->adj_list->end(); ++i){
    //     if(!(*i)->visited){
    //         fila.push_back((*i)->value);
    //         (*i)->visited = true;
    //     }
    // }
    bool parada = true;
    //percorrindo a fila
    list<int>::iterator verticeFila = fila.begin();
    while(parada){
        cerr<< "while bloqueia" << endl;
        for(list<Node*>::iterator i = vetor[*verticeFila]->adj_list->begin(); 
            i != vetor[*verticeFila]->adj_list->end(); ++i){
            cerr<< "for bloqueia" << endl;
            cerr<< "olhando vertice "<< (*i)->value << " " << (*i)->visited << endl;
            //achou o gateway
            if((*i)->gateway){
                cerr<< "primeiro if" << endl;
                cout << (*i)->value << " " << *verticeFila << endl;
                parada = false;
                //excluir aresta marcada
                break;
            }
            if(!(*i)->visited){
                cerr<< "segundo if" << endl;
                fila.push_back((*i)->value);
                (*i)->visited = true;
            }
        }
        ++verticeFila;
        if(verticeFila == fila.end()){
            parada = false;
        }
    }
}
 
int main()
{
    int n; // the total number of nodes in the level, including the gateways
    int l; // the number of links
    int e; // the number of exit gateways
    cin >> n >> l >> e; cin.ignore();
    //    cerr << "primeira entrada"<< endl;
    for (int i = 0; i < n; i++) {
        Node* nodeAux = (Node*)malloc(sizeof(Node));
        nodeAux->value = i;
        nodeAux->adj_list = new list<Node*>[l];
        //cout << nodeAux->adj_list.size() << endl;
        vetor.push_back(nodeAux);
    }
   
    cerr << "tamanho do vetor " <<vetor.size() << endl;
    for (int i = 0; i < l; i++) {
        int n1; // N1 and N2 defines a link between these nodes
        int n2;
        cin >> n1 >> n2; cin.ignore();
        n1;
        n2;
        cerr << "aresta " << n1 << " - " << n2 << endl;
        cerr << "vetor[" << n1 << "] = " << vetor[n1]->value << endl;
        cerr << "vetor[" << n2 << "] = " << vetor[n2]->value<< endl;
        //cout << typeid(vetor[n1]).name() << endl;
        try{
            vetor[n1]->adj_list->push_back(vetor[n2]);
            vetor[n2]->adj_list->push_back(vetor[n1]);
        }catch(exception& e){
            cerr << e.what() <<endl;
        }
    }
    cerr << "criou as arestas"<< endl;
   
    for (int i = 0; i < e; i++) {
        int ei; // the index of a gateway node
        cin >> ei; cin.ignore();
        vetor[ei]->gateway=true;
    }
    cerr << "criou os gateway"<< endl;
 
    // game loop
    while (1) {
        int si; // The index of the node on which the Bobnet agent is positioned this turn
        cin >> si; cin.ignore();
        vetor[si]->enemy = true;
        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        bloqueia(si);
        cerr << "entrou no while"<< endl;
        // Example: 0 1 are the indices of the nodes you wish to sever the link between
        //cout << "0 1" << endl;
        vetor[si]->enemy = false;
    }
}


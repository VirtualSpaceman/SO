/*
Trabalho Opcional de Sistemas Operacionais
Aluno: Levy Gurgel Chaves
Matrícula: 1332330

*/
#include <bits/stdc++.h>

using namespace std;

struct treeNode
{
    string name;
    vector <treeNode> vec;
};



template<typename Out>
void split(const string &s, char delim, Out result)
{
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        *(result++) = item;
    }
}

vector<string> split(const string &s, char delim)
{
    vector<string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}

treeNode* searchNode(treeNode *no, string name)
{

    for(unsigned int i = 0; i < no->vec.size(); i++){
        if(no->vec[i].name == name)
            return &no->vec[i];
    }
    return new treeNode;
}

bool searchTree(treeNode *head, string gram)
{

    treeNode *n = head;//new treeNode;
    vector <string> aux = split(gram, ' ');
    //n->vec.push_back(*head);

    int depth = aux.size();
    int cont = 1;

    while(cont != depth){
       n = searchNode(n, aux[cont]);
       if(n->name == "")
            return false;

        cont++;
    }

    aux.clear();
    return true;
}

treeNode init(string name){

    treeNode aux;
    aux.name = name;
    return aux;
}


void insertTree(treeNode *head, string gram,int depth)
{

    if(depth == 1 and gram != head->name){
        head->name = gram;
        return;
    }
    else{
        treeNode *aux = head;
        treeNode *teste;
        int i = 1;
        vector<string> v = split(gram, ' ');
        while(depth != 1){
            teste = searchNode(aux, v[i]); //Se achar, retorna o treeNode
            if(teste->name == ""){ //Se for igual � pq n�o achou o que eu tava querendo
                aux->vec.push_back(init(v[i]));
                //(find(aux->vec.begin(), aux->vec.end(), value)) !=  aux->vec.end()
                //cout << v[i] << endl;
            }
            else
                aux = teste;
            i++;
            depth--;
        }
    }

}


void inVec(vector <treeNode> &vec, string nam)
{

    vec.push_back(init(nam));
}

void showVal(treeNode *head){

    for(unsigned int i = 0; i < head->vec.size() ; i++){
        cout << head->vec[i].name<< "  " << head->vec[i].vec.size() << endl;
        showVal(&head->vec[i]);
    }
}

vector<treeNode*> build_root(vector<string> vec){
  vector <string> s;
  vector<treeNode*> v;

  for(int i=0; i < vec.size()-1; i++){
    if(find(s.begin(), s.end(), vec[i]) == s.end()){
      s.push_back(vec[i]);
      v.push_back(new treeNode{vec[i]});
    } //s nao contem o elemento
  }

  s.clear();

  return v;
}

treeNode* find_root(vector<treeNode*> roots, string gram){
    string aux = gram.substr(0, gram.find(" "));
    for(unsigned int i = 0; i < roots.size(); i++){
      if(roots[i]->name == aux)
        return roots[i];
    }
}

void score_grams(){

    int score = 0; //Score de infecçao
    bool aux;  //Variavel auxliar
    int size = 0; //Quantide de n-grams em um arquivo de texto
    ifstream uninfected_file; //Arquivo sadio
    ifstream infected_file; //Arquivo infectado
    uninfected_file.open("/home/levy/Programming/mkdirgram.txt");
    infected_file.open("/home/levy/Programming/lsgram.txt");

    cout << "\n\n           O programa \"ls\" contem 78 subsequencias com n-gram = 10 " << endl;
    cout << "           O programa \"mkdir\" contem 67 subsequencias com n-gram = 10 " << endl;
    cout << "           Arquivo contaminado no teste: Programa \"ls\"" << endl;
    cout << "           Arquivo sadio no teste: Programa \"mkdir\"" << endl;

    if(infected_file.is_open() and uninfected_file.is_open()){
      string token_inf;
      string token_uninf;
      //Realiza a contagem do score. Nenhuma estrutura auxiliar foi utilizada, apenas a leitura do arquivo.
      while(getline(infected_file, token_inf)){
        aux = false;
        size++;
        while(getline(uninfected_file, token_uninf)){
          if(token_inf == token_uninf){
            aux = true;
            break;
          }
        }
        if (!aux){
          score++;
        }
      }
    }
    //Mostra o score e fecha dos arquivos abertos para a leitura
    cout << "           Score: " <<double(score)/double(size)*100 << "%\n\n\n"<< endl;
    uninfected_file.close();
    infected_file.close();
}

void get_n_gram(int n_gram){

  vector<treeNode*> roots;
  map<string, int> mp; //Estrutura para evitar system calls repetidas
  ifstream arquivo; //arquivo de entrada(para ler)
  ofstream outfile; //Arquivo de saída (armazena resultados)
  int qtde_sys = 0; //Variável para contar a quantidade
  vector <string> syscall_vector; //Vetor que contem todas as subsequencias de n-gram

  arquivo.open("/home/levy/Programming/lsproc.txt"); //Arquivo processado do item a)
  //outfile.open("/home/levy/Programming/lsgram.txt"); //Arquivo onde será armazenado os n-grams

  if(arquivo.is_open()){
    string line;
    while(getline(arquivo, line)){ //Coloca todas as system calls do programa em um vetor
      string token = line.substr(0, line.find(" "));
      syscall_vector.push_back(token);
      qtde_sys++;
    }
  }

  roots = build_root(syscall_vector);
  //return;

  for (int j = 2; j <= n_gram; j++){

    unsigned int qtd = qtde_sys - j + 1; //Quantidades de n-grams que serão gerados.
    unsigned int k = 0; //usado como auxiliar para extrair as n-grams
    int cont = 1; //Variavel para n deixar repetidas entrarem
    while(qtd--){//Realiza o algoritmo de janela deslizante
      string gram = "";
      for(unsigned int i = k; i < k+j; i++){
          gram = gram + syscall_vector[i] + " ";
      }
      gram.pop_back();
      if(mp[gram] == 0){ //Verificação para evitar que n-grams repetidas sejam escritas no arquivo
        mp[gram] = cont;
        cont++;
        //cout << "Gram de tamanho " << j << "(" << cont-1 << ") " <<  ": " <<gram << endl;
        //treeNode *q = find_root(roots, gram);
        insertTree(find_root(roots, gram), gram, j);
        //outfile << gram << endl; //Escreve no arquivo as subsequencias não repetidas
      }

      k++;
    }
    mp.clear();
  }
  //Fecha os arquivos abertos para leituras e escritas.

  if(searchTree(find_root(roots, "execve brk access mmap access open fstat mmap close access"), "execve brk access mmap access open fstat mmap close access")){
    cout << "aaaaaaaaaaaaaaaaa" << endl;
   }
    showVal(roots[0]);

  syscall_vector.clear();
  arquivo.close();
  outfile.close();
  mp.clear();

}

int main(int argc, char const *argv[]) {

  get_n_gram(10);
//  score_grams();

  return 0;
}

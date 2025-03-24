#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <algorithm>

using namespace std;

int duracao;

class Nodo {
	public:
		string posicao;
		Nodo *pai;
		Nodo *filho;
		// contrutor que recebe o pai se nao receber sera NULL
		Nodo (string n = "0,0,", Nodo *p = NULL, Nodo *f = NULL){
			posicao = n;
			pai = p;
			filho = f;
		}
		~Nodo(){
			#ifdef DEBUG
			cout << "Nodo destruido" << endl;
			#endif
		}
		Nodo *getPai (){
			//retorna o pai, durrr
			return pai;
		}
		// retorna a posicao!
		string getPosicao (){
			return posicao;
		}
		void setFilho (Nodo *f){
			filho = f;
		}
		void *getPaiprincipal(){
			Nodo *aux = this;
			while (1){
				aux->pai->filho = aux;
				aux = aux->pai;
				if (aux->pai == NULL){
					return aux;
				}
			}
		}
		// faz um while que percorre o nodo indo de pai em pai, ate achar o grande adao!
		string getCaminho (){
			stringstream ss;
			//nodo auxiliar se torna igual ao objeto atual
			Nodo *aux = this;
			Nodo *aux2 = this->getPai();
			while (1){
				ss << aux->getPosicao();
				if (aux->getPai() == NULL){
					break;
				}
				aux = aux->getPai();
			}
			return ss.str();
		}
};

class Labirinto{
	private:
		vector <vector <char>> labirinto;
		int linha;
		int tamColuna;
		stringstream DFS;
	public:
		// construtor que seta a posicao inicial com X  e recebe o labirinto
		Labirinto (vector <vector <char>> lab){
			labirinto = lab;
			labirinto[0][0] = 'X';
			linha = 0;
		}
		// destrutor
		~Labirinto(){
			#ifdef DEBUG
			cout << "Labirinto destruido" << endl;
			#endif
		}
		// printa o labirinto
		void printLabirinto (){
			string RED = "\033[31m";
			string RESET = "\033[0m";
			for (int i = 0; i < labirinto.size(); i++) {
				for (int j = 0; j < labirinto[i].size(); j++) {
					if (labirinto[i][j] == 'X'){
						cout<<RED<<labirinto[i][j]<<RESET;
					}
					else{
						cout<<labirinto[i][j];
					}
				}
			cout<<endl;
			}
		}
		void mudarTabuleiro (int linha, int coluna){
			labirinto[linha][coluna] = 'X';
		}
		// recebe o caminho completo formatado da maneira ("linha","coluna")
		// atualizando o tabuleiro verdadeiro para o caminho desejado
		int atualizarTabuleiro (string caminho){
			bool auxiliar = false;
			double tamanho = 0;

			stringstream ss(caminho);
			string aux;

			int linha1;
			int coluna1;

			//parte teste de implementacao
			chrono::milliseconds duration(duracao);

			// varre o input (caminho) ate achar "," ou o final
			// atribui primeiro a linha depois coluna, sempre alternando entre as duas
			    while (getline(ss, aux, ',')) {
					if (auxiliar == false){
						istringstream(aux) >> linha1;
						auxiliar = true;
					}
					else{
						istringstream(aux) >> coluna1;
						auxiliar = false;
						mudarTabuleiro(linha1, coluna1);
						this_thread::sleep_for(duration);
						system ("clear");
						printLabirinto();
					}
				}
				return tamanho;
		}
		vector <vector <char>> getTabuleiro (){
			return labirinto;
		}
		int tamanhoCaminho (){
			int caminho = 0;
			for (int i = 0; i < labirinto.size(); i++) {
				for (int j = 0; j < labirinto[i].size(); j++) {
					if (labirinto[i][j] == 'X'){
						caminho++;
					}
				}
			}
			return caminho;
		}
};

class speedRunnerdeLabirinto{
	private:
		//inteiros que vao receber a posicao atual
		int linha,coluna;
		//linked list que funciona se referindo ao seu pai
		vector <Nodo *> fila;
		//labirinto auxiliar que da pra rabiscar a vontade
		vector <vector <char>> labirinto;
		//indice para evitar verificacoes desnecessarias de filas ja passadas
		int indice,indiceDFS;
		//bools que representam os movimentos possiveis
		bool right,left,down,up,vitoria;
		vector <string> pilha;
		stringstream DFScaminho;
	public:
		//recebe o tabuleiro
		//importante falar que ta meio macaco essa implementacao, mas funciona entao ta tudo bem!
		speedRunnerdeLabirinto (vector <vector <char>> lab){
			//atribui o tabuleiro a devida variavel
			labirinto = lab;
			//inicializa linha com a posicao inicial
			fila.push_back(new Nodo("0,0,", NULL));
			//transforma string em inteiros
			transformarString(linha, coluna, fila[0]->getPosicao());
			up = false;
			right = false;
			down = false;
			left = false;
			indice = 0;
			vitoria = false;
			indiceDFS = 0;
		}
		//destrutor destroi todas filas criadas
		~speedRunnerdeLabirinto(){
			for (int i = 0; i < fila.size(); i++) {
				delete fila[i];
			}
			#ifdef DEBUG
			cout << "speedRunnerdeLabirinto  destruido" << endl;
			#endif
		}
		//seta todos possiveis movimentos para false
		void turnFalse(bool &u, bool &d, bool &r, bool &l){
			u = false;
			r = false;
			d = false;
			l = false;
		}
		// pega todos as possibilidades de movimentos e da pushback nas novas linhas, referenciando seu devido pai
		void encherNodo(bool &u, bool &d, bool &r, bool &l, const int linha, const int coluna){
			if (u == true){
				fila.push_back(new Nodo((to_string(linha-1) + "," + to_string(coluna) + "," ), fila[indice]));
			}
			if (d == true){
				fila.push_back(new Nodo((to_string(linha+1) + "," + to_string(coluna) + "," ), fila[indice]));
			}
			if (r == true){
				fila.push_back(new Nodo((to_string(linha) + "," + to_string(coluna+1) + "," ), fila[indice]));
			}
			if (l == true){
				fila.push_back(new Nodo((to_string(linha) + "," + to_string(coluna-1) + "," ), fila[indice]));
			}
			indice++;
		}
		//funcao que transforma em string o local onde o speedrunner esta
		void transformarString (int &linha, int &coluna, string posicao){
			//bool auxiliar para diferenciar leitura para coluna ou linha
			bool auxiliar = false;

			// transfora a posicao em uma stream
			stringstream ss(posicao);
			// pega o primeiro token
			string aux;
				//while que le ate o final do input, ou encontrar um ,
			    while (getline(ss, aux, ',')) {
					// comeca com auxiliar em false para passar o primeiro valor para linha
					if (auxiliar == false){
						istringstream(aux) >> linha;
						auxiliar = true;
					}
					// em uma segunda passagem pelo while na vai cair no if e atribuir o restante do input a coluna
					else{
						istringstream(aux) >> coluna;
					}
				}
		}
		//testa todas possibilidades de movimentos, atribuindo a cada variavel true ou false, dependendo se o movimento pode ser realizado ou nao
		//se o movimento for possivel vai mudar a posicao para X, impedindo que seja possivel passar por la novamente
		void testarPossiblidades (bool &u, bool &d, bool &r, bool &l, const int linha, const int coluna){
				if (linha == labirinto.size()-1){
					d = false;
				}
				else if (labirinto[linha+1][coluna] == '.'){
					labirinto[linha+1][coluna] = 'X';
					d = true;
				}
				if (linha == 0){
					u = false;
				}
				else if (labirinto[linha-1][coluna] == '.'){
					labirinto[linha-1][coluna] = 'X';
					u = true;
				}
				if (coluna == labirinto[linha].size()-1){
					r = false;
				}
				else if (labirinto[linha][coluna+1] == '.'){
					labirinto[linha][coluna+1] = 'X';
					r = true;
				}
				if (coluna == 0){
					l = false;
				}
				else if (labirinto[linha][coluna-1] == '.'){
					labirinto[linha][coluna-1] = 'X';
					l = true;
				}
		}
		// funcao que checa os arredores do local, se qualquer um deles for F, vai retornar true e vai adicionar o nodo a fila 
		//com a posicao final do labirinto, se nao vai retornar false
		bool testarVitoria (const int linha, const int coluna){
				if (linha == labirinto.size()-1){
				}
				else if (labirinto[linha+1][coluna] == 'F'){
					fila.push_back(new Nodo((to_string(linha+1) + "," + to_string(coluna) + "," ), fila[indice]));
					return true;
				}
				if (linha == 0){
				}
				else if (labirinto[linha-1][coluna] == 'F'){
					fila.push_back(new Nodo((to_string(linha-1) + "," + to_string(coluna) + "," ), fila[indice]));
					return true;
				}
				if (coluna == labirinto[linha].size()-1){
				}
				else if (labirinto[linha][coluna+1] == 'F'){
					fila.push_back(new Nodo((to_string(linha) + "," + to_string(coluna+1) + "," ), fila[indice]));
					return true;
				}
				if (coluna == 0){
				}
				else if (labirinto[linha][coluna-1] == 'F'){
					fila.push_back(new Nodo((to_string(linha) + "," + to_string(coluna-1) + "," ), fila[indice]));
					return true;
				}
			return false;
		}
		string BFS (){
			while (1){
				turnFalse(up,down,right,left);
				transformarString(linha, coluna, fila[indice]->getPosicao());
				if (testarVitoria(linha,coluna) == true){
					break;
				}
				testarPossiblidades(up,down,right,left,linha,coluna);
				encherNodo(up,down,right,left,linha,coluna);
			}
			return fila[fila.size()-1]->getCaminho();
			}
		bool DFS (int const linha1, int const coluna12){
			pilha.push_back(to_string(linha1) + "," + to_string(coluna12) + ",");
			labirinto [linha1][coluna12] = 'X';
			bool u = false;
			bool d = false;
			bool r = false;
			bool l = false;
			if (testarVitoria(linha1,coluna12) == true){
				fila[fila.size()-1]->getPaiprincipal();
				return true;
			}
			testarPossiblidades(u,d,r,l,linha1,coluna12);
			if (u == true){
				if (DFS(linha1-1, coluna12)){
					return true;
				}
			}
			if (d == true){
				if (DFS(linha1+1, coluna12)){
					return true;
				}
			}
			if (r == true){
				if (DFS (linha1,coluna12+1)){
					return true;
				}
			}
			if (l == true){
				if (DFS (linha1,coluna12-1)){
					return true;
				}
			}
			pilha.pop_back();
			return false;
		}
		string getCaminhoDFS(){
			stringstream ss;
			for(int i = 0; i < pilha.size(); i++){
				ss<<pilha[i];
			}
			return ss.str();
		}
		};

int main (int duracao1, char** argv){
	duracao = stoi(argv[1]);
        std::string str = argv[2];  // Converte o primeiro argumento para `std::string`
        std::cout << "Argumento recebido: " << str << std::endl;
	cout << str;
	//leitura do arquivo lab.text, transformando ele em uma matriz de caracteres
    ifstream file(str);

    if (!file.is_open()) {
        cerr << "Failed to open the file" << endl;
        return 1;
    }

    std::vector<std::string> linhas;
    std::string linha;

    while (std::getline(file, linha)) {
        linhas.push_back(linha);
    }

    int linha2 = linhas.size();
    int coluna = (linha2 > 0) ? linhas[0].length() : 0;

    vector<vector<char>> matriz(linha2, vector<char>(coluna)); // Initialize the matriz with the correct dimensions

    for (int i = 0; i < linha2; i++) {
        for (int j = 0; j < coluna; j++) {
            matriz[i][j] = linhas[i][j];
        }
    }

    file.close();

	//cria o objeto labirinto inicializando com a matriz recem captada 
	Labirinto lab(matriz);

	//cria o objeto speedRunnerdeLabirinto com o objeto labirinto
	speedRunnerdeLabirinto speed(matriz);

	//usa o metodo BFS do objeto speedRunnerdeLabirinto para encontrar o caminho e capta-lo como uma string
	string caminho = speed.BFS();


	//usa o metodo DFS do objeto speedRunnerdeLabirinto para encontrar o caminho
	//bool teste = speed.DFS(0,0);
	//capta o caminho encontrado
	//string caminho = speed.getCaminhoDFS();

	//atualiza o qlabirinto com o caminho captado
	lab.atualizarTabuleiro(caminho);
	//printa o labirinto atualizado
	//lab.printLabirinto();
	
	return 0;
}
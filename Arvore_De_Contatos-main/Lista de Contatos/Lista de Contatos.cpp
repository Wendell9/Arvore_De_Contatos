#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <limits>
using namespace std;

class Contato {
public:
    Contato(std::string nome, std::string telefone, std::string email, bool fav) : Nome(nome), Telefone(telefone), Email(email), Favorito(fav) {}

    std::string getNome() const {
        return Nome;
    }

    std::string getTelefone() const {
        return Telefone;
    }

    std::string getEmail() const {
        return Email;
    }

    bool GetFavorito() const {
        return Favorito;
    }

    friend std::ostream& operator<<(std::ostream& os, const Contato& pessoa) {
        os << "Nome: " << pessoa.Nome << "\n";
        os << "Email: " << pessoa.Email << "\n";
        os << "Telefone: " << pessoa.Telefone << "\n";
        if (pessoa.Favorito) {
            os << " Favoritado" << "\n";
        }
        return os;
    }

private:
    std::string Nome;
    std::string Telefone;
    std::string Email;
    bool Favorito;
};

class Node {
public:
    Contato contato;
    Node* left;
    Node* right;
    int height;

    Node(Contato c) : contato(c), left(nullptr), right(nullptr), height(1) {}
};

class BinaryTree {
private:
    Node* root;

public:
    BinaryTree() : root(nullptr) {}

    void insercao(Contato value) {
        root = insert(root, value);
    }

    void leituraEmOrdem() {
        inorder(root);
        std::cout << std::endl;
    }

    void desenharArvore() {
        std::cout << drawTree(root, "");
    }

    void remover(std::string nome) {
        root = remove(root, nome);
    }

    void exportarParaCSV(std::ofstream& out) {
        exportInOrder(root, out);
    }

    Contato* search(Node* node, std::string nome) {
        if (node == nullptr || node->contato.getNome() == nome) {
            return node ? &node->contato : nullptr;
        }
        if (nome < node->contato.getNome()) {
            return search(node->left, nome);
        }
        return search(node->right, nome);
    }

    Contato* buscar(std::string nome) {
        return search(root, nome);
    }
private:
    Node* insert(Node* node, Contato c) {
        if (node == nullptr) {
            return new Node(c);
        }

        if (c.getNome() < node->contato.getNome()) {
            node->left = insert(node->left, c);
        }
        else {
            node->right = insert(node->right, c);
        }

        // Atualiza a altura do nó
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

        // Calcula o fator de balanceamento do nó
        int balance = getBalance(node);

        // Casos de rotação
        if (balance > 1 && c.getNome() < node->left->contato.getNome()) {
            return rotateRight(node);
        }
        if (balance < -1 && c.getNome() > node->right->contato.getNome()) {
            return rotateLeft(node);
        }
        if (balance > 1 && c.getNome() > node->left->contato.getNome()) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && c.getNome() < node->right->contato.getNome()) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        // Realiza a rotação
        x->right = y;
        y->left = T2;

        // Atualiza alturas
        y->height = 1 + std::max(getHeight(y->left), getHeight(y->right));
        x->height = 1 + std::max(getHeight(x->left), getHeight(x->right));

        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        // Realiza a rotação
        y->left = x;
        x->right = T2;

        // Atualiza alturas
        x->height = 1 + std::max(getHeight(x->left), getHeight(x->right));
        y->height = 1 + std::max(getHeight(y->left), getHeight(y->right));

        return y;
    }

    int getHeight(Node* node) {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    int getBalance(Node* node) {
        if (node == nullptr)
            return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    void inorder(Node* node) {
        if (node == nullptr)
            return;
        inorder(node->left);
        std::cout << node->contato << " ";
        inorder(node->right);
    }

    void exportInOrder(Node* node, std::ofstream& out) {
        if (node == nullptr)
            return;
        exportInOrder(node->left, out);
        out << node->contato.getNome() << "," << node->contato.getTelefone() << "," << node->contato.getEmail() << "," << (node->contato.GetFavorito() ? "Sim" : "Não") << "\n";
        exportInOrder(node->right, out);
    }

    Node* remove(Node* node, std::string nome) {
        if (node == nullptr) {
            std::cout << "Contato '" << nome << "' não encontrado." << std::endl;
            return nullptr;
        }

        if (nome < node->contato.getNome()) {
            node->left = remove(node->left, nome);
        }
        else if (nome > node->contato.getNome()) {
            node->right = remove(node->right, nome);
        }
        else {
            if (node->left == nullptr || node->right == nullptr) {
                Node* temp = node->left ? node->left : node->right;
                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                }
                else {
                    *node = *temp;
                }
                delete temp;
            }
            else {
                Node* temp = menorValorNode(node->right);
                node->contato = temp->contato;
                node->right = remove(node->right, temp->contato.getNome());
            }
        }
        if (node == nullptr)
            return node;

        // Atualiza a altura do nó
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

        // Calcula o fator de balanceamento do nó
        int balance = getBalance(node);

        // Casos de rotação
        if (balance > 1 && getBalance(node->left) >= 0) {
            return rotateRight(node);
        }
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && getBalance(node->right) <= 0) {
            return rotateLeft(node);
        }
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }

    Node* menorValorNode(Node* node) {
        Node* atual = node;
        while (atual->left != nullptr) {
            atual = atual->left;
        }
        return atual;
    }

    // Corrigido o tipo de retorno para string
    std::string drawTree(Node* node, std::string prefix) {
        if (node == nullptr)
            return "";

        std::string output;

        output += drawTree(node->right, prefix + "    ");
        output += prefix + node->contato.getNome() + "\n";
        output += drawTree(node->left, prefix + "    ");

        return output;
    }
};

class Sistema {
public:
    BinaryTree arvoreAVL;
    list<Contato> Favoritos;

    void Tela() {
        string input;
        bool hasLetter;
        int opcoes;

        do {
            do {
                MenuOpcoes();
                cin >> input;
                for (char c : input) {
                    if (std::isalpha(c)) {
                        hasLetter = true;
                        break;
                    }
                    else
                    {
                        hasLetter = false;
                    }
                }

                if (hasLetter) {
                    std::cout << "Valor invalido. Favor escolher um valor que corresponda ao range de opcoes" << std::endl;
                    cin.ignore();
                }
            } while (hasLetter);

            opcoes = std::stoi(input);
            switch (opcoes) {
            case 1:
                AdicionarContato();
                break;
            case 2:
                RemoverContato();
                break;
            case 3:
                BuscarContato();
                break;
            case 4:
                ListarContatos();
                break;
            case 5:
                ListarFavoritos();
                break;
            case 6:
                ImportarCSV();
                break;
            case 7:
                ExportarCSV();
                break;
            case 8:
                arvoreAVL.desenharArvore();
                break;
            case 9:
                break;
            default:
                break;
            }
        } while (opcoes != 9);
    }

    void RemoverFavorito(string nome) {
        // Usando remove_if para remover o contato com o nome especificado
        Favoritos.remove_if([nome](const Contato& contato) { return contato.getNome() == nome; });
    }

    void ListarFavoritos() {
        for (const auto& contato : Favoritos) {
            cout << contato << endl;
        }
    }

    void MenuOpcoes() {
        cout << "1 - Adicionar Contatos" << endl;
        cout << "2 - Remover Contatos" << endl;
        cout << "3 - Buscar Contatos" << endl;
        cout << "4 - Listar Contatos" << endl;
        cout << "5 - Listar Favoritos" << endl;
        cout << "6 - Importar CSV" << endl;
        cout << "7 - Exportar CSV" << endl;
        cout << "8 - Mostrar o desenho da arvore" << endl;
        cout << "9 - Sair" << endl;
    }

        void AdicionarContato() {
        string nome, email, telefone;
        int favorito; // Alterado para inteiro
        bool hasLetter = false;
        do {
            cin.ignore();
            hasLetter = false;
            cout << "Nome: ";
            getline(cin, nome);
            cout << "Email: ";
            getline(cin, email);
            cout << "Telefone: ";
            getline(cin, telefone);
            cout << "Favoritar contato? (1 - Sim; 0 - Nao): ";
            cin >> favorito;



            for (char c : telefone) {
                if (std::isalpha(c)) {
                    hasLetter = true;
                    break;
                }
            }

            if (hasLetter) {
                std::cout << "Numero de Telefone invalido. Ha letras no numero inserido" << std::endl;
            }

        } while (hasLetter);


        if (favorito == 1) {
            arvoreAVL.insercao(Contato(nome, telefone, email, true));
            Favoritos.push_back(Contato(nome, telefone, email, true));
        }
        else {
            arvoreAVL.insercao(Contato(nome, telefone, email, false));
        }
    }

    void RemoverContato() {
        string nome;
        cout << "Qual o nome do contato que você deseja remover? ";
        cin >> nome;
        arvoreAVL.remover(nome);
        RemoverFavorito(nome); // Remover da lista de favoritos também
    }

    void ListarContatos() {
        arvoreAVL.leituraEmOrdem();
    }

    void ExportarCSV() {
        ofstream outFile("contatos_exporta.csv");
        if (!outFile) {
            cerr << "Não foi possível abrir o arquivo para escrita." << endl;
            return;
        }

        outFile << "Nome,Telefone,Email,Favorito\n";
        arvoreAVL.exportarParaCSV(outFile);

        outFile << "\nFavoritos:\n";
        for (const auto& contato : Favoritos) {
            outFile << contato.getNome() << "," << contato.getTelefone() << "," << contato.getEmail() << "," << (contato.GetFavorito() ? "Sim" : "Não") << "\n";
        }

        outFile.close();
        cout << "Contatos exportados com sucesso para 'contatos_exporta.csv'." << endl;
    }

    void ImportarCSV() {
        ifstream inFile("contatos_importa.csv");
        if (!inFile) {
            cerr << "Não foi possível abrir o arquivo para leitura." << endl;
            return;
        }

        string line;
        bool isFavoritos = false;

        while (getline(inFile, line)) {
            if (line.empty()) continue; // Pula linhas vazias
            if (line == "Favoritos:") {
                break;
            }

            if (line == "Nome,Telefone,Email,Favorito") {
                continue; // Pula o cabeçalho
            }



            stringstream ss(line);
            string nome, telefone, email, favoritoStr;
            bool favorito = false;

            getline(ss, nome, ',');
            getline(ss, telefone, ',');
            getline(ss, email, ',');
            getline(ss, favoritoStr, ',');

            favorito = (favoritoStr == "Sim");

            Contato contato(nome, telefone, email, favorito);
            arvoreAVL.insercao(contato);
            if (favorito) {
                Favoritos.push_back(contato);
            }
        }

        inFile.close();
        cout << "Contatos importados com sucesso de 'contatos_importa.csv'." << endl;
    }

    void BuscarContato() {
        string nome;
        cout << "Digite o nome do contato que você deseja buscar: ";
        cin >> nome;

        Contato* contato = arvoreAVL.buscar(nome);
        if (contato) {
            cout << *contato << endl;
        }
        else {
            cout << "Contato não encontrado." << endl;
        }
    }
};

int main() {
    Sistema sistema;

    sistema.Tela();

    return 0;
}

#include <vector>
#include <iostream>
#include <fstream>

struct Node {
	int value;
	std::vector<Node*> children;
};


class NTree {
private:
	Node* root;

	Node* createNode(int value, int childCount) {
		Node* newNode = new Node();
		newNode->value = value;
		newNode->children.resize(childCount, nullptr);
		return newNode;
	}

	Node* readTreeInner(std::istream& stream) {
		int nodeValue;
		int childCount;

		if (!(stream >> nodeValue) || !(stream >> childCount) || childCount < 0) {
			throw std::runtime_error("Invalid input!!!");
		}

		Node* newNode = createNode(nodeValue, childCount);

		for (int i = 0; i < childCount; i++) {
			newNode->children[i] = readTreeInner(stream);
		}

		return newNode;
	}

	static void printTreeInner(const Node* node, int depth) {
		if (node == nullptr) {
			return;
		}

		std::cout << std::string(depth * 4, ' ') << node->value << '\n';

		for (const Node* child : node->children) {
			printTreeInner(child, depth + 1);
		}
	}

	bool isSameStructure(Node* localRoot, Node* refRoot) {
		if (localRoot == nullptr || refRoot == nullptr) {
			return localRoot == refRoot;
		}

		if (localRoot->children.size() != refRoot->children.size()) {
			return false;
		}

		for (int i = 0; i < localRoot->children.size(); i++) {
			if (!isSameStructure(localRoot->children[i], refRoot->children[i])) {
				return false;
			}
		}
	
		return true;
	}

	void findSubtreesInner(Node* localRoot, Node* refRoot, std::vector<Node*>& finded) {
		if (localRoot == nullptr) {
			return;
		}

		if (isSameStructure(localRoot, refRoot)) {
			finded.push_back(localRoot);
			return;
		}

		for (int i = 0; i < localRoot->children.size(); i++) {
			findSubtreesInner(localRoot->children[i], refRoot, finded);
		}
	}

	void destroyTree(Node* node) {
		if (node == nullptr) {
			return;
		}

		for (Node* child : node->children) {
			destroyTree(child);
		}

		delete node;
	}

public:
	NTree(){
		root = nullptr;
	}
	~NTree() {
		destroyTree(root);
	}

	void readTree(std::istream& stream) {
		if (root == nullptr) {
			root = readTreeInner(stream);
		}
		else {
			destroyTree(root);
			root = readTreeInner(stream);
		}
	}

	Node* getRoot() const {
		return root;
	}

	static void printTree(const Node* localRoot) {
		printTreeInner(localRoot, 0);
	}

	std::vector<Node*> findSubtrees(const NTree& reference) {
		std::vector<Node*> finded;
		Node* refRoot = reference.getRoot();
		findSubtreesInner(root, refRoot, finded);
		return finded;
	}
};


int main() {
	while (true) {
		try {
			std::cout << "Where do I get the tree? \n1 - console\n2 - file\n0 - exit\n\nEnter: ";
			int switchValue;
			NTree tree;
			NTree templateTree;
			if (!(std::cin >> switchValue)) {
				throw std::runtime_error("Invalid menu option");
			}
			switch (switchValue) {
			case 1:
			{
				std::cout << "Enter the main tree: ";
				tree.readTree(std::cin);
				std::cout << "Enter the template tree: ";
				templateTree.readTree(std::cin);

				break;
			}
			case 2: {
				std::cout << "Enter the main tree filename: ";
				std::string filename;
				std::cin >> filename;

				std::ifstream file(filename);
				if (!file) {
					throw std::runtime_error("File error!!!");
				}
				tree.readTree(file);
				file.close();
				
				std::cout << "Enter the template tree filename: ";
				std::string templateFilename;
				std::cin >> templateFilename;

				std::ifstream templateFile(templateFilename);
				if (!templateFile) {
					throw std::runtime_error("File error!!!");
				}
				templateTree.readTree(templateFile);
				templateFile.close();

				break;
			}
			case 0: {
				return 0;
			}
			default: {
				throw std::runtime_error("Unknown menu option");
			}
			}
			std::cout << "MAIN TREE: \n";
			NTree::printTree(tree.getRoot());
			std::cout << "TEMPLATE TREE STRUCTURE: \n";
			NTree::printTree(templateTree.getRoot());

			std::vector<Node*> findedSubtrees = tree.findSubtrees(templateTree);
			std::cout << "FINDED SUBTREES: ";
			for (int i = 0; i < findedSubtrees.size(); i ++) {
				std::cout << "\n" << i << ": \n";
				NTree::printTree(findedSubtrees[i]);
			}

			std::cout << std::endl;
			std::cout << "\nPress Enter to continue...\n";
			std::cin.ignore(30, '\n');
			std::cin.get();
			system("cls");
		}
		catch (const std::exception& error) {
			std::cout << "\nError: " << error.what() << "\n\n";
			if (std::cin.eof()) {
				return 0;
			}
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
}
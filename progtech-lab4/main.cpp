#include <vector>
#include <iostream>
#include <fstream>


// Ввод дерева реализован в следующем формате:
// Вводится пара значений. Первое - значение данной ноды. Второе - количество детей этой ноды.
// После чего происходит ввод всех детей ноды

struct Node {
	int value;
	std::vector<Node*> children;
};


class NTree {
private:
	Node* root;
	std::vector<int> levels;
	int maxLevel;

	Node* createNode(int value, int childCount) {
		Node* newNode = new Node();
		newNode->value = value;
		newNode->children.resize(childCount, nullptr);
		return newNode;
	}

	Node* readTreeInner(std::istream& stream) {
		int nodeValue;
		int childCount;

		stream >> nodeValue;
		stream >> childCount;
		if (childCount < 0) {
			throw std::runtime_error("Invalid input!!!");
		}


		Node* newNode = createNode(nodeValue, childCount);

		for (int i = 0; i < childCount; i++) {
			newNode->children[i] = readTreeInner(stream);
		}

		return newNode;
	}

	void calcLevelContentInner(Node* localRoot, int curLevel) {
		if (localRoot == nullptr) {
			return;
		}

		if (levels.size() <= curLevel) {
			levels.resize(levels.size() + 1);
			maxLevel = levels.size();
		}
		levels[curLevel] += 1;
		for (int i = 0; i < localRoot->children.size(); i++) {
			calcLevelContentInner(localRoot->children[i], curLevel + 1);
		}
	}

	void printTreeInner(const Node* node, int depth) {
		if (node == nullptr) {
			return;
		}

		std::cout << std::string(depth * 4, ' ') << node->value << '\n';

		for (const Node* child : node->children) {
			printTreeInner(child, depth + 1);
		}
	}

public:
	NTree(){
		root = nullptr;
	}

	void readTree(std::istream& stream) {
		root = readTreeInner(stream);
		calcLevelContentInner(root, 0);
	}

	void printTree() {
		printTreeInner(root, 0);
	}
};



int main() {
	while (true) {
		try {
			std::cout << "Where do I get the tree? \n1 - console\n2 - file\n0 - exit\n\nEnter: ";
			int switchValue;
			NTree tree;
			if (!(std::cin >> switchValue)) {
				throw std::runtime_error("Invalid menu option");
			}
			switch (switchValue) {
			case 1:
			{

			}
			case 2: {
				std::cout << "Enter the filename: ";
				std::string filename;
				std::cin >> filename;

				std::ifstream file(filename);
				tree.readTree(file);
				break;
			}
			case 0: {
				return 0;
			}
			default: {
				throw std::runtime_error("Unknown menu option");
			}
			}

			tree.printTree();

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
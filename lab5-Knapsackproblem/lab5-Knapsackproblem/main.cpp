#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;



//created a knapsack class with the fillKnapsack function
//url:https://www.byte-by-byte.com/01knapsack/






// Item struct
 struct Item {
    string name;
    unsigned int weight;
    unsigned int value;

     Item(unsigned int sWeight, unsigned int sValue, string sName) {
        name = sName;
        weight = sWeight;
        value = sValue;
    }
};

 void printVector(std::vector <Item> const a) {


	 std::cout << "\nThe store items are : \n\n";

	 for (unsigned int i = 0; i < a.size(); i++)
		 std::cout << "weight : " << a[i].weight << " value : $" << a[i].value << " " << a[i].name << ' ' << endl;

	 cout << "\n";
 }

class Knapsack
{

private:
    unsigned int maxWeight;
    

public:
	vector<Item> myItems;

	Knapsack(unsigned int weight) {
		maxWeight = weight;
		
	}

	int returnWeight() {
		return maxWeight;
		
	}

	void updateWeight(int newWeight) {
		maxWeight += newWeight;

	}

	int returnLootSize() {
		return myItems.size();
	
	}


    int fillKnapsack(vector<Item> items){

    // cache[i][j] = max value for the first i items with a max weight of j
     vector<vector<unsigned int> > cache(items.size() + 1, vector<unsigned int>(maxWeight + 1, 0));

    //here we begin to fill our cache with the best optimal solution for each sub problem
    for(unsigned int i = 1; i <= items.size(); i++) {

        for (unsigned int j = 0; j <= maxWeight; j++) {
            //Check if including item[i-1] would exceed max weight j
            //if not
            if (items[i-1].weight > j) cache[i][j] = cache[i-1][j];

            //if not then compare values between including or excluding the item
            else cache[i][j] = max(cache[i-1][j], cache[i-1][j-items[i-1].weight] + items[i-1].value);
            }
    }
        //need to add code for adding objects to myItems and displaying the objects that you selected.
        //source url: https://www.geeksforgeeks.org/printing-items-01-knapsack/
        unsigned int w = maxWeight;
        unsigned int n = items.size();
        int count = 0;
        unsigned int res = cache[items.size()][maxWeight];;
            for (unsigned int i = n; i > 0 && res > 0; i--) {

                // either the result comes from the top
                // (K[i-1][w]) or from (val[i-1] + K[i-1]
                // [w-wt[i-1]]) as in Knapsack table. If
                // it comes from the latter one/ it means
                // the item is included.
                if (res == cache[i - 1][w])
                    continue;
                else {

                    // This item is included.
                    cout << "\nitem " << count+1 << " in your backpack ---> " << items[i - 1].name << " worth : " << items[i-1].value;
					count++;
					myItems.push_back(items[i - 1]);
                    // Since this weight is included its
                    // value is deducted
                    res = res - items[i - 1].value;
                    w = w - items[i - 1].weight;
                }
            }
			cout << "\nyou took $" << cache[items.size()][maxWeight] << " worth of stuff";
            cout << "\n\n\n";
			return cache[items.size()][maxWeight];
	}
};

class Robber {
private:
	string name;
	Knapsack * lootBag;
	int bankAccount;

public:
	Robber(string sName) {

		name = sName;
		bankAccount = 0;
		lootBag = new Knapsack(5);
	}

	~Robber() {
	
		delete lootBag;

	}

	string returnName() {
		return name;
	
	}

	void upgradeBag() {
		lootBag->updateWeight(1);
	}

	int returnBankSum() {
		return bankAccount;
	
	}

	void donateToCharity() {
		bankAccount = 0;
	}

	void printMyItems() {

		if (lootBag->myItems.size() > 0) {
			for (unsigned int i = 0; i < lootBag->myItems.size(); i++) {
				cout << lootBag->myItems[i].name << "\n";
			}
		}
		else {
		
			cout << "You have no Items!\n\n";
		
		}
	}

	bool bagFull() {
		
		return lootBag->returnLootSize() > 0;
	}

	int returnMaxWeight() {
		return lootBag->returnWeight();

	}

	void rob(vector<Item> items) {

		lootBag->fillKnapsack(items);
	}

	void sellItems() {
		for (unsigned int i = 0; i < lootBag->myItems.size(); i++) {
			bankAccount += lootBag->myItems[i].value;
		}

		lootBag->myItems.clear();
	}
};


int main()
{
	//items for the stores
    Item pancakeMix(1,6,"Pancake Mix");
    Item potatoes(2,10,"Sac o potatoes");
    Item nachoPlatter(3,12,"Nacho Platter");
	Item Lasagna(2, 8, "Lasagna");
	Item WidescreenTV(5, 18, "4k TV");
	Item Playstation4(4, 15, "PS4 Pro");
	Item WireLessKeyboard(3, 9, "WireLess Keyboard");
	Item GTX1080ti(3, 21, "GTX 1080ti");
	Item GamingLaptop(1, 8, "Gaming Laptop");
	Item GsyncMonitor(2, 11, "G sync Monitor");
	Item goldBrick1(1, 6, "Gold Brick");
	Item silverBrick1(2, 10, "Silver Brick");
	Item goldCoins(3, 16, "Bag of gold coins");

    vector<Item> Winco;
    Winco.push_back(pancakeMix);
    Winco.push_back(potatoes);
    Winco.push_back(nachoPlatter);

	vector<Item> BestBuy;
	BestBuy.push_back(WidescreenTV);
	BestBuy.push_back(Playstation4);
	BestBuy.push_back(WireLessKeyboard);
	BestBuy.push_back(GTX1080ti);
	BestBuy.push_back(GsyncMonitor);
	BestBuy.push_back(WireLessKeyboard);
	BestBuy.push_back(GamingLaptop);

	vector<Item> ChaseBankVault;
	ChaseBankVault.push_back(goldBrick1);
	ChaseBankVault.push_back(silverBrick1);
	ChaseBankVault.push_back(goldCoins);

   
	string nameInput;
	cout << "Hello what is your name?\n";
	cin >> nameInput;
	
	Robber robber1(nameInput);

	cout << "\nHello " << robber1.returnName() << "\n\n";
	
	Robber Reggie("Reggie");
	

	int input = 0;
	std::cout << "Welcome to the streets. It's a cold world out here where we must rob and steal to survive\n";
	std::cout << "It's not all bad though because you can sell your stolen goods and give back to charity, pshhh\n\n";

	do
	{
		
		cout << "You have $" << robber1.returnBankSum() << " in your bank account\n\n";
		cout << "Items currently in your bag\n";
		robber1.printMyItems();

		std::cout << "\nWhat would you like to do?\n\nPlease make a selection: \n";
		std::cout << "1) Rob a Store\n";
		std::cout << "2) Sell Items\n";
		std::cout << "3) Buy Items\n";
		std::cout << "4) Donate to charity\n";
		std::cout << "0) Quit\n";
		std::cin >> input;

		if (input ==  1)

		{   
			if (robber1.bagFull()) {
			
				cout << "Your lootbag is full. Sell items before attempting to fill again.\n\n";
			}
			else {

				//Display stores to rob
				cout << "\nHere are the businesses in town, which would you like to rob?\n";
				std::cout << "1) Winco\n";
				std::cout << "2) BestBuy\n";
				std::cout << "3) Chase Bank Vault\n";

				int selection;
				cin >> selection;

				if (selection == 1) {

					printVector(Winco);

					cout << "\nYour bag can hold up to " << robber1.returnMaxWeight() << " lbs------------------\n\n";
					cout << "Here are the items you stole\n";
					robber1.rob(Winco);

				}

				if (selection == 2) {

					printVector(BestBuy);

					cout << "\nYour bag can hold up to " << robber1.returnMaxWeight() << " lbs------------------\n\n";
					cout << "Here are the items you stole\n";
					robber1.rob(BestBuy);

				}

				if (selection == 3) {

					printVector(ChaseBankVault);

					cout << "\nYour bag can hold up to " << robber1.returnMaxWeight() << " lbs------------------\n\n";
					cout << "Here are the items you stole\n";
					robber1.rob(ChaseBankVault);

				}
				

			}


		}

		if (input == 2) {
			robber1.sellItems();
		
		}
		if (input == 3) {
			cout << "\nWhat do you want to buy?\n1)bag upgrade $100\n2)out of stock\n3)out of stock\n";

			int selection;

			cin >> selection;

			if (selection == 1) {
				
				if (robber1.returnBankSum() < 100) {
				
					cout << "not enough money leave this store nowww!!!\n";
				}
				else {
					robber1.upgradeBag();
					cout << "Your bag has been updated. Your max weight is now " << robber1.returnMaxWeight() << "\n";
				}

			}
			else {
				cout << "item out of stock choose again\n";
			}
		
		}
		if (input == 4) {

			robber1.donateToCharity();

			cout << "Congratulations!! You've donated your easy earned money to charity this should clear your conscious for now.\n";
			cout << "Don't worry you did a good thing, just refrain from robbing, turn your life around and make better decisions\n\nGood luck in life\n";
		
		}



	} while (input != 0);
    




}

#include <iostream>
#include <string>

#include "Dealer.h"

using namespace std;

int showMenu(string header, string *choices, int numChoices, bool showBalance, int balance) {
    if (header != "") cout << "=====" << header << "=====" << endl;
    if (showBalance) cout << "Current Balance: $" << balance << endl;
	for (int i = 0; i < numChoices; i++) cout << i + 1 << ". " << choices[i] << endl;
    string choiceString;
    getline(cin, choiceString);
    int choice = -1;
    try {
    	choice = stoi(choiceString);
    } catch (...) {}
    if (choice < 1 || choice > numChoices) {
        cout << "Invalid choice. Please enter the number of a menu option." << endl;
        return showMenu(header, choices, numChoices, showBalance, balance);
    }
    return choice;
}

int betInput(bool showBalance, int balance) {
	if (showBalance) cout << "Current Balance: $" << balance << endl;
    cout << "Enter bet:" << endl;
    string betString;
    getline(cin, betString);
    int bet = -1;
    try {
    	bet = stoi(betString);
    } catch (...) {}
    if (bet < 1 || bet > balance) {
        cout << "Invalid bet. Please enter an integer >= 1 and <= " << balance << "." << endl;
        return betInput(false, balance);
    }
    return bet;
}

void printPlayerHand(bool dealer, bool dealerHidden, Hand playerHand) {
    cout << (dealer ? "Dealer" : "Player") << " hand:" << endl;
    int numCards = playerHand.getCards().size();
    for (int i = 0; i < numCards; i++) {
        if (dealer && dealerHidden && numCards == 2 && i == 0) cout << "?" << endl;
        else cout << playerHand.getCards()[i].toString() << endl;
    }
    cout << "Value: " << playerHand.getValue(dealerHidden) << endl;
    cout << endl;
}

int main() {
	int balance = 500;
	Hand playerHand;
	Dealer dealer(&playerHand);
	while (1) {
    	string mainChoices[2] = {"Place bet", "Quit"};
    	int mainChoice = showMenu("Welcome to Blackjack!", mainChoices, 2, true, balance);
    	if (mainChoice == 1) {
    		int initialBet = betInput(true, balance);
    		cout << endl;
    		printPlayerHand(true, true, dealer.getHand());
			printPlayerHand(false, false, playerHand);
			if (playerHand.getValue(false) == 21) {
				cout << "Blackjack!" << endl;
				balance += initialBet * 1.5;
			}
			bool firstBet = true;
			if (balance - (initialBet * 2) < 0) firstBet = false;
			while (1) {
				string betChoices[3] = {"Hit", "Stay", "Double Down"};
    			int betChoice = showMenu("", betChoices, (firstBet ? 3 : 2), false, 0);
    			cout << endl;
    			if (betChoice == 1) {
    				firstBet = false;
    			    dealer.hit(&playerHand);
    			    printPlayerHand(false, false, playerHand);
    			    if (playerHand.getValue(false) > 21) break;
    			} else if (betChoice == 2) {
    				dealer.stay();
    			    break;
    			} else if (betChoice == 3) {
    				initialBet *= 2;
    			    dealer.doubleDown(&playerHand);
    			    break;
    			}
			}
			printPlayerHand(true, false, dealer.getHand());
			printPlayerHand(false, false, playerHand);
			int playerFinalValue = playerHand.getValue(false);
			int dealerFinalValue = dealer.getHand().getValue(false);
			if (playerFinalValue > 21) {
    			cout << "Bust!" << endl;
    			balance -= initialBet;
    		}
    		else if (playerFinalValue == dealerFinalValue) cout << "Push" << endl;
    		else if (dealerFinalValue > 21) {
    			cout << "Dealer Busts. You win!" << endl;
    			balance += initialBet;
    		}
    		else if (playerFinalValue < dealerFinalValue) {
    			cout << "Dealer wins!" << endl;
    			balance -= initialBet;
    		}
    		else if (playerFinalValue > dealerFinalValue) {
    			cout << "You win!" << endl;
    			balance += initialBet;
    		}
    	} else if (mainChoice == 2) {
    	    cout << "Goodbye!" << endl;
    	    break;
    	}
    }
    return 0;
}

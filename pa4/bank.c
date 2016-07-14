#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*bank.c*/
/*gcc -o bank bank.c*/


enum Boolean{true , false};

struct Account{
	char name[100];
	float currBal;
	enum Boolean inSesh;
};


int capacity = 0;
struct Account *accts[20];

void accountStart(){
	int i;
	for( i = 0; i < 20; i++){
		accts[i] = 0;
	}
}

char* createAccount(char* name){
	char *message = malloc(sizeof(char)*100);
	if(strlen(name) > 100){
		message = "Error: Name is over 100 letters when creating new account\n";
		return message;
	}
	if(capacity >= 20){
		message = "Error: Too many clients are in attached to the server\n";
		return message;
	}
	int i = 0;
	while(i < 20){
		if(accts[i] != NULL && strcmp(accts[i]->name, name) == 0){
			message = "Error: Two Accounts with the same name\n";
			return message;
		}
		i++;
	}
	
	if(capacity <  20){
			int a = 0;
			while(a < 20){
				if(accts[a] == NULL){
					accts[a] = malloc(sizeof(struct Account));
					strcpy(accts[a]->name, name);
					accts[a]->currBal = 0;
					accts[a]->inSesh = false;
					capacity++; 
					strcpy(message,name);
					return message;
				}
				a++;
			}
	}
	message = "Error:\n";
	return message;

}

char* withdraw(char* name, float amount){
	char *message = malloc(sizeof(char)*100);
	if(strlen(name) > 100){
		message = "\nError: Name is over 100 letters when creating new account\n";
		return message;
	}
	if (amount < 0) {
		message = "\nYour deposit amount is invalid";
		return message;
	}
	int i;
	for( i = 0; i < 20; i++){
		if(accts[i] != NULL && strcmp(accts[i]->name, name) == 0){
			if(accts[i]->currBal < amount){
				message = "\nError: Insufficient Funds for Withdraw\n";
				return message;
			}
			else{
				accts[i]->currBal -= amount;
				//printf("Amount in account: %f\n", accts[i]->currBal);
				message = "\nYou have sucessfully withdrawn the money\n";
				return message;
			}	
		}
	}
	message = "\nError: Trying to Withdraw from name that does not exist\n";
	return 0; 
}






char* deposit(char* name, float amount){
	char *message = malloc(sizeof(char)*100);
	if(strlen(name) > 100){
		message = "\nError: Name is over 100 letters when creating new account\n";
		return message;
	}
	if (amount < 0) {
		message = "\nYour deposit amount is invalid\n";
		return message;
	}
	int i = 0;
	while(i < 20){
		//printf("%s\n", "into the loop");
		if(accts[i] != NULL && strcmp(accts[i]->name, name) == 0){
			//printf("%s%f\n","The actutal amount:::::::::::::" ,amount );
			float balance = accts[i]->currBal;
			accts[i]->currBal = balance + amount;
			message = "\nYou have been able to deposit\n";
			printf("%s%f\n","Your current acount balance is: " ,accts[i]->currBal);
			return message;
		}
		i++;
	}
	
	message = "\nName you gave is not in accounts\n";
	return message;
}


char* query(char* name){
	//char buf[100];
	char *message = malloc(sizeof(char)*100);
	int i;
	for( i = 0; i < 20; i++){
		if(accts[i] != NULL && strcmp(accts[i]->name, name) == 0){
			//printf("%s%f\n", "current account balance: ",accts[i]->currBal);
			snprintf (message, sizeof(message) + 1, "%f", accts[i]->currBal);
			//message = 
			return message;                                                        
		}	
	}
	message = "\nError: Name does not exist for query call\n";
	return message;			
}

char* isaccountthere(char *name){
	char *message = malloc(sizeof(char)*100);
	int i;
	for( i = 0; i < 20; i++){
		if(accts[i] != NULL && strcmp(accts[i]->name, name) == 0){
			strcpy(message,name);
			return message;	
		}
	}
	message = "wrong";
	return message;
}






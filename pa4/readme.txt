Vamsi Anamaneni and Jessie Strivelli

readme for PA4
---------------
to use:
-make
-./server3
-./client3 127.0.0.1
-uses port 5281

client3:
1st will ask to enter a # or press anything to start banks server
   -preferrably type in hi
2nd to create/server account
   -type in 0
   -then type in a name
3rd to deposit 
   -type in 1
   -then type in a name
   -then type in amount of money to deposit
4th to withdraw 
   -type in 3
   -then type in a name
   -then type in amount of money to withdraw
5th to query
   -type in 3
   -type in name
6th to quit
    -type in 4
----------------------------------------------
bank.c

-our data structure is a struct caled Account which holds
     -name - a persons name
     -float currBal - holds current balance of an account
     -a Boolean inSesh - to tell if account is in session or not

-created a array of structs call accts

our methods
void accountStart()
Sets all of the values of the account struct for all 20 accounts to 0.

char* createAccount(char* name)
Takes in a name and looks if the name is already there, which sends an error. If the name
is not there, it will create a new account corresponding to the given name as long as
there are less than 20 accounts, if more than 20 it will send an error.

char* withdraw(char* name, float amount)
takes in a name, searches for it, if not found an error. If found,
also uses given amount to withdraw money from the account,
if the amount needed to withdraw is more than in the account
there will be an error or if amount to withdraw is less than 0, there will be an error.
If success will send back a message saying You have sucessfully withdrawn the money.

char* deposit(char* name, float amount)
akes in a name, searches for it, if not found an error. If found,
also uses given amount to deposit money in the account,
if amount to deposit is less than 0, there will be an error.
If success will send back a message saying You have been able to deposit.

char* query(char* name).
takes in a name and searches for the account, if account is not there, sends an error,
if account is there, sends back the corresponding account's bank balance. 

char* isaccountthere(char *name)
takes in a name, searches for corresponding account, if name is there returns the name
otherwise returns the message "wrong".
----------------------------------------------------------------------------------
server3.c

in the client_session_thread, the server will read in messages from the client
and call the appropriate function from bank.c
and send the appropriate response back to the client.

------------------------------------------------------------
client3.c

Takes in commands from user and sends them to server.c
Throttles the input every 2 seconds.
If unable to connect to server, tries every 3 seconds.

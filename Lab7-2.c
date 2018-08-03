#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

 
void printBoard(char board[][26], int n); //prints board template
bool positionInBounds(int n, char row, char col); //checks wether the move is in bounds of the board
bool checkLegalInDirection(char board[][26], int n, char row, char col,char colour, int deltaRow, int deltaCol); //checks if move is legal
bool valid(char board[][26], int n, char row, char col,char colour); //checks if move is valid in each direction
int maxScore(char board[][26], int n, char colour); //keeps track of the highest score
int score(char board[][26],int n,char row,char col, char color); //computes the scores
void makeMove(char board[][26],int n,char row, char col,char colour); //flips tiles in the board
void computer(char board[][26], int n, char colour); //determines when the coputer should make a move
bool gameOver(char board[][26], int n); //checks if theres any tiles left or valid moves
bool noMoreMoves(char board[][26], int n,char colour); //checks if any moves are available
void startGame(char board[][26],int dimensions,char computerColor); //plays the whole game with the human vs computer
void pickWinner(char board[][26], int n); //chooses winner based on number of tiles
void possibleMoves(char board[][26], int n, char colour); //finds the availabe moves for either colour
char decideColour(); //asks user for which colour computer should play

 

int main(void)
{
    
    int dimensions;
    char board[26][26]; //sets a 2D array for the initial board
    char computerColor,moveRow,moveCol;
    char configColour,configRow,configColumn; 

    printf("Enter the board dimension:");
    scanf("%d",&dimensions);

    computerColor = decideColour(); //asks user for which colour computer should play

    //prints the board initially with W and B's in the center
    for (int row=1;row<=dimensions;row++){
        for(int column=1;column<=dimensions;column++){
            if ((row== dimensions/2 && column== dimensions/2) || (row == (dimensions/2)+1 && column== (dimensions/2)+1)){
                board[row-1][column-1]='W';
            } else if ((row== dimensions/2 && column== (dimensions/2)+1) || (row == (dimensions/2)+1 && column == (dimensions/2))){
                board[row-1][column-1]='B';
            } else board[row-1][column-1]='U';
        }
    }

    printBoard(board,dimensions); //prints board
    startGame(board,dimensions,computerColor); //plays the game between the computer and player

    return 0;
}

 
//used to input color of computer(putting it directly in main was causing errors)
char decideColour() {
    
    char computerColour; 
    printf("Computer plays (B/W) :"); 
    scanf(" %c",&computerColour);
    return computerColour; 

}


void startGame(char board[][26],int dimensions,char computerColor) {
   
    char moveRow,moveCol;
    bool defaultOver=true;    

    if(computerColor=='B') { //if the computer is B, the computer plays first
        while(!gameOver(board,dimensions)) { //while the game is not over
            if(!noMoreMoves(board,dimensions,'B')){ //checks to see if there are moves available
                computer(board,dimensions,'B'); //computer makes first move
                printBoard(board,dimensions);
            } else if (!gameOver(board,dimensions)) (printf("B player has no valid move.\n"));  //if no more valid moves but game still running

            if(!noMoreMoves(board,dimensions,'W')){ //checks to see if there are moves available
                printf("Enter move for colour W (RowCol):");
                scanf(" %c%c",&moveRow, &moveCol); //gets user input
                if(moveRow=='!' && moveCol=='!') { 
                    possibleMoves(board,dimensions,'W'); //checks possible moves
                    printf("Enter move for colour W (RowCol):");
                    scanf(" %c%c",&moveRow, &moveCol);
                }

                if (valid(board,dimensions,moveRow,moveCol,'W')) { //if the move is valid
                    makeMove(board,dimensions,moveRow,moveCol,'W'); //makes the move
                } else {
                    printf("Invalid move.\nB player wins.\n"); //breaks loop if user inputs invalid move
                    defaultOver=false;
                    break;
                }
                
                printBoard(board,dimensions); //prints the board

            } else if  (!gameOver(board,dimensions)) (printf("W player has no valid move.\n")); //shows winner once game is over
        }
    } else if(computerColor=='W') { //similar process to the one before but in different order
        while(!gameOver(board,dimensions)) {
             if (!noMoreMoves(board,dimensions,'B')) {
                printf("Enter move for colour B (RowCol):"); //user goes first 
                scanf(" %c%c",&moveRow, &moveCol);
                if(moveRow=='!' && moveCol=='!') {
                    possibleMoves(board,dimensions,'B');
                    printf("Enter move for colour B (RowCol):");
                    scanf(" %c%c",&moveRow, &moveCol);
                }

                if (valid(board,dimensions,moveRow,moveCol,'B')) {
                    makeMove(board,dimensions,moveRow,moveCol,'B'); 
                } else {
                    printf("Invalid move.\nW player wins.\n");
                    defaultOver=false;
                    break;
                }
                printBoard(board,dimensions); //prints the board

             } else if (!gameOver(board,dimensions)) (printf("B player has no valid move.\n"));

            if (!noMoreMoves(board,dimensions,'W')) { //if there are moves for W
                computer(board,dimensions,'W'); //computer will play
                printBoard(board,dimensions); //prints the board
            } else if (!gameOver(board,dimensions)) (printf("W player has no valid move.\n"));       

        }

    }

    if(defaultOver) pickWinner(board,dimensions); //output the winner

}
 

void printBoard(char board[][26], int n) {
    printf("  ");
    for(int row = 0; row <= n; row++){ // runs for 'dimension+1' times
        for (int coloum = 0; coloum < n; coloum++) { // runs for 'dimension' times
            if(row == 0) printf("%c",'a' + coloum);
            else {
                if(coloum == 0) printf("%c %c",'a'+ row-1,board[row-1][coloum]); //since columns run 1 less times than rows, first part of board printed with row indicator
                else printf("%c",board[row-1][coloum]); //[row-1] because row 0 is for the column indicator
            }
        }
        printf("\n");
    }
} 


bool positionInBounds(int n, char row, char col) {
    int numRow= row-'a';//gets numberic value for row from character starting from 0
    int numCol= col-'a';//gets numberic value for column from character starting from 0
     if ((numRow < n && numRow >= 0) && (numCol < n && numCol >= 0)) { //checks to see if position is within the bound dimensions of the array
        return true; 
    }
    return false;
    
}
 

bool checkLegalInDirection(char board[][26], int n, char row, char col,char colour, int deltaRow, int deltaCol) { 

     int numRow= row-'a';//gets numberic value for row from character starting from 0
     int numCol= col-'a';//gets numberic value for column from character starting from 0
     char opposition;
     if (colour== 'W') opposition='B'; //sets opposition color depending on the current color being used
     if (colour== 'B') opposition='W';     

     if(positionInBounds(n,row+deltaRow,col+deltaCol)) {
         do{
             if (board[numRow+deltaRow][numCol+deltaCol]==opposition){//if adjacent block of opposition color, checks the next block
                 numRow= numRow+deltaRow;
                 numCol= numCol+deltaCol;
             }

             else if(board[numRow+deltaRow][numCol+deltaCol]==colour && board[numRow][numCol]==opposition) return true;// returns true when a opposite color is followed by the color of choice
             else break;// break to prevent infinite loop
         } while((numRow>=0 && numRow <n) && (numCol>=0 && numCol<n));
         
     } //closing bracket for if statement

     return false;
}
 

bool valid(char board[][26], int n, char row, char col,char colour) {
    for (int x=-1;x<=1;x++){
        for (int y=-1;y<=1;y++){ //for loops to generate different combinations of direction
            if (!(x==0 && y==0) && checkLegalInDirection(board,n,row,col,colour,x,y)) return true;//ignore origing (0,0) and checks if move is legal
        }
    } return false;
}
 

int maxScore(char board[][26], int n, char colour) {
    int maxScore=0;

    for (int row = 0; row < n; row ++) {
        for (int col = 0; col< n; col++) {//runs through the 2D board array
            if(board[row][col] == 'U' && valid(board,n,'a'+row,'a'+col,colour)) { //sees in which empty spots can a move be made
                int points=score(board,n,'a'+row,'a'+col,colour);
                if (points>maxScore){
                    maxScore=points;
                }
            }
        } 
    }  return maxScore;
}


//computer makes a move  
void computer(char board[][26], int n, char colour) {
    int max= maxScore(board,n,colour);
    int points=0,temp=0; //initialize points and temp counter
    for(int row=0;row<n;row++){
        for(int col=0;col<n;col++){
            if(board[row][col] == 'U' && valid(board,n,'a'+row,'a'+col,colour)) {
                points= score(board,n,'a'+row,'a'+col,colour); //counts the points on the path
                if (points==max && temp<max){
                    makeMove(board,n,'a'+row,'a'+col,colour); //if points is max, computer should make a move
                    printf("Computer places %c at %c%c.\n",colour,'a'+row,'a'+col);
                    temp=max+1;
                }
            }
        }
    }
}

 
//calculates the score
int score(char board[][26],int n,char row,char col, char color) {    

    int numRow= row-'a';//gets numberic value for row from character starting from 0
    int numCol= col-'a';//gets numberic value for column from character starting from 0
    int score=0;    

    if(board[numRow][numCol] == 'U' && valid(board,n,row,col,color)) {
        for (int x=-1;x<=1;x++) {
            for (int y=-1;y<=1;y++) {
                if(!(x==0 && y==0) && checkLegalInDirection(board,n,row,col,color,x,y)) {
                    int numRow= row-'a';//gets numberic value for row from character starting from 0
                    int numCol= col-'a';//gets numberic value for column from character starting from 0 
                    while(board[numRow+x][numCol+y]!=color) {
                        numRow+=x;
                        numCol+=y;
                        score=score+1; //score counter
                    }
                }
            }
        }
    }
    return score;
}

 
//flips board tiles
void makeMove(char board[][26],int n,char row,char col, char colour) {    

    int numRow= row-'a';//gets numberic value for row from character starting from 0
    int numCol= col-'a';//gets numberic value for column from character starting from 0    

    if(board[numRow][numCol] == 'U' && valid(board,n,row,col,colour)) {
        board[numRow][numCol]=colour;//sets chose move to choses color
        for (int x=-1;x<=1;x++) {
            for (int y=-1;y<=1;y++) {
                if(!(x==0 && y==0) && checkLegalInDirection(board,n,row,col,colour,x,y)) {
                    int numRow= row-'a';//gets numberic value for row from character starting from 0
                    int numCol= col-'a';//gets numberic value for column from character starting from 0 
                    while(board[numRow+x][numCol+y]!=colour) {
                        numRow+=x;
                        numCol+=y;
                        board[numRow][numCol]=colour; //flips all the other blocks in between to chosen color
                    }
                }
            } 
        }
    }

    else printf("Invalid move.\n"); // alerts of invalid move

}

 
//checks if any moves are available
bool noMoreMoves(char board[][26], int n,char colour) {

    int count=0;
    for (int row = 0; row < n; row ++) {
        for (int col = 0; col< n; col++) {//runs through the 2D board array
            if(board[row][col] == 'U' && valid(board,n,'a'+row,'a'+col,colour)){ //sees in which empty spots can a moved me made
                count=count+1;
            }
        }
    }

    if (count==0) {
        return true;
    }
    else return false;

}

//determines when game is over
bool gameOver(char board[][26], int n) {

    int countEmpty=0; 
    for (int row=0;row<n;row++){ //checks to see if there are anymore available moves or empty tiles
        for (int col=0;col<n;col++){
            if (board[row][col]=='U') countEmpty+=1;
        }
    }

    if(countEmpty==0 || (noMoreMoves(board,n,'W') && noMoreMoves(board,n,'B'))) return true;
    else return false;

}

 
//chooses winner based on num of tiles
void pickWinner(char board[][26], int n) {
    
    int countBlack=0,countWhite=0;
    for (int row=0;row<n;row++) {
        for (int col=0;col<n;col++) {
            if (board[row][col]=='B') countBlack+=1; //score for black
            if (board[row][col]=='W') countWhite+=1; //score for white
        }
    }

    if (countBlack>countWhite) printf("B player wins.\n");
    else if (countWhite>countBlack) printf("W player wins.\n");
    else if (countBlack==countWhite) printf("Draw!\n");

}

 
//finds the possible moves
void possibleMoves(char board[][26], int n, char colour) {
    //printf("Available moves for %c:\n",colour);
    for (int row = 0; row < n; row ++) {
        for (int col = 0; col< n; col++) {//runs through the 2D board array
            if(board[row][col] == 'U' && valid(board,n,'a'+row,'a'+col,colour)) { //sees in which empty spots can a move be made
                //printf("%c%c\n",'a'+row,'a'+col);
            }
        }
    }
}

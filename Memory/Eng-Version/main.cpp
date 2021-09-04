#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstring>
using namespace std;
#define CARD_WIDTH 5
#define CARD_HEIGHT 5

struct CARD{
    char* Value = NULL;
    bool opened=false, gathered=false;
};

CARD* cards=NULL;
int width, height, player_points[2], which_player;
bool over;

bool unique_chars(char** pictures, int index){
    for (int i = 0; i < index; i++) {
        if (strcmp(pictures[i], pictures[index]) == 0)
            return true;
    }
    return false;
}

void shuffling(char** pictures, int dimension){
    for (int i = 0; i < dimension - 1; i++) {
        int j = (rand() % (dimension - i)) + i;
        char* t = pictures[j];
        pictures[j] = pictures[i];
        pictures[i] = t;
    }
}

int  power(int a, int b){
    if (b == 0) return 1;
    for (int i = 0; i < b - 1; i++)
        a = a * a;
    return a;
}

void draw(){
    system("CLS");
    for (int i = 0; i < height; i++) {
        cout << "-";
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < CARD_WIDTH; k++) {
                cout << "-";
            }
            cout << "-";
        }
        cout << "\n";

        for (int k = 0; k < CARD_HEIGHT; k++) {
            cout << "|";
            for (int j = 0; j < width; j++) {
                for (int l = 0; l < CARD_WIDTH; l++) {
                    if (cards[i * width + j].gathered) {
                        cout << " ";
                        continue;
                    };

                    if (k == 0 || k == CARD_HEIGHT - 1 || l == 0 || l == CARD_WIDTH - 1) {
                        if (cards[i * width + j].opened) {
                            cout << "/";
                        }
                        else
                            cout << "#";
                    }
                    else {
                        if (!cards[i * width + j].opened) {
                            int a = i * width + j + 1;
                            int x = (a / power(10, (CARD_HEIGHT - 1) - k - 1)) % 10;
                            if ((x != 0 || ((CARD_HEIGHT - 1) - k == 1 && a >= 10) || (CARD_HEIGHT - 2) - k == 1 && a>=100) && l == CARD_WIDTH / 2) {
                                cout << x;
                            }
                            else {
                                cout << " ";
                            }
                        }
                        else {
                            if (CARD_HEIGHT - k - 2 < 3 && l == CARD_WIDTH / 2)
                                cout << cards[i * width + j].Value[CARD_HEIGHT - k - 2];
                            else
                                cout << " ";
                        }
                    };
                }
                cout << "|";
            }
            cout << "\n";
        }
    }
    for (int j = 0; j < width; j++) {
        for (int k = 0; k < CARD_WIDTH + 1; k++) {
            cout << "-";
        }
    }
    cout << endl << endl;
    cout << "Collected cards: \nPLAYER 1: " << player_points[0] << "\nPLAYER 2: " << player_points[1] << "\n\n";
    cout << "CURRENT PLAYER: " << which_player + 1 << "\n\n";
}

void draw_pictures(){
    char** pictures = new char* [height * width];
    for (int j = 0; j < height * width / 2;) {
        pictures[j] = new char[4];
        for (int i = 0; i < 3; i++) {
            int number = (rand() % 33) + 222;
            pictures[j][i] = number;
        }
        pictures[j][3] = '\0';
        if (!unique_chars(pictures, j)) j++;
    }
    memcpy(pictures + height * width / 2, pictures, sizeof(char*) * height * width / 2);
    int dimension = height * width;
    shuffling(pictures, dimension);
    for (int i = 0; i < height * width; i++) {
        if (cards[i].Value != NULL)
            delete cards[i].Value;
        cards[i].Value = pictures[i];
    }
    delete[] pictures;
}

void sizing(){
    while (true) {
        system("CLS");
        char X[500];
        cout << "Welcome to the game 'MEMORY'!!! \nChoose the dimensions of your board \nThese are the rules you need to follow: " << endl << endl;
        cout << "Dimensions cannot be \n- Odd (meaning that at least one number must be even)  \n- Equal 0 \n- 2x1, 1x2 or 2x2 \n- Less than 0 \n- The number of columns cannot be greater than 35" << endl << endl;
        cout << "Diemensions: \n Columns = "; cin >> X;
        width = atoi(X);
        cout << " Rows = "; cin >> X;
        height = atoi(X);
        if ((width <= 2 && height <= 2) || (width>=36) || (width>=36 && height>=29)) {
            cout << "\nPlease enter the correct dimensions.\n";
            cout << "\nHit enter to try again.\n";
            cin.get(); cin.get();
            continue;
        }
        if (width % 2 != 0 && height % 2 != 0) {
            cout << "\nOne dimension must be even.\n";
            cout << "\nHit enter to try again.\n";
            cin.get(); cin.get();
            continue;
        }
        break;
    }
    which_player = 0;
    delete[] cards;
    cards = new CARD[height * width];
    player_points[0] = player_points[1] = 0;
    draw_pictures();
    draw();
}

void choice(){
    char which[500];
    int number;
    draw();

    cout << "Check the first card with the number: "; cin >> which;
    number = atoi(which);
    if (number <= 0 || number > height * width) {
        cout << "\nEnter a valid card number.\n\n";
        return;
    }
    CARD* card1 = &cards[number - 1];
    if ((*card1).opened || (*card1).gathered) {
        cout << "This card has already been selected.\n\n";
        (*card1).opened = false;
        return;
    }
    (*card1).opened = true;
    draw();

    cout << "Check the second card with the number: "; cin >> which;
    number = atoi(which);
    if (number <= 0 || number > height * width) {
        cout << "Enter a valid card number.\n\n";
        (*card1).opened = false;
        return;
    }
    CARD* card2 = &cards[number - 1];
    if ( (*card2).opened || (*card2).gathered ) {
        cout << "This card has already been selected.\n\n";
        (*card1).opened = false;
        return;
    }
    (*card2).opened = true;
    draw();

    int values = strcmp((*card1).Value, (*card2).Value);
    if (values == 0) {
        cout << "The cards are correct! You get them.\n";
        (*card1).gathered = true;
        (*card2).gathered = true;
        player_points[which_player] += 2;
    }
    else {
        cout << "Cards don't match.\n";
    }

    cout << "\nPress any button to switch players.\n";
    which_player = (which_player + 1) % 2;
    (*card1).opened = false;
    (*card2).opened = false;
    cin.get(); cin.get();
    draw();
}

int  main(){
    over = false;
    srand(time(NULL));
    sizing();
    while (!over) {
        char character;
        if (player_points[0] + player_points[1] != height * width) {
            cout << "s - Start the queue; r - Restart; q - Quit\n";
            cin >> character;
            switch (character)
            {
            case 'q': {
                over = true; break;
            }
            case 's': {
                choice(); break;
            }
            case 'r': {
                sizing(); break;
            }
            default: {
                draw(); break;
            }
            }
        }
        else {
            if (player_points[0] == player_points[1]) {
                cout << "DRAW!";
            }
            else {
                cout << "PLAYER NUMBER ";
                if (player_points[0] > player_points[1])
                    cout << " 1 HAS WON!!! CONGRATS!!!";
                else
                    cout << " 2 HAS WON!!! CONGRATS!!!";
            }
            cout << "\n\nR - Restart; Q - Quit\n";
            cin >> character;
            switch (character)
            {
            case 'q': {
                over = true; break;
            }
            case 'r': {
                sizing(); break;
            }
            }
            draw();
        }
    }
    delete[] cards;
}
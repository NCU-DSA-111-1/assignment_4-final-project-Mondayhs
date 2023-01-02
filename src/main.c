#include <stdio.h>

int buy_fruit(money)
{

    int amount;
    printf("How many do you want to buy?");
    scanf("%d", &amount);

    // if (amount < 0)
    // {
    //     printf("You can't buy negative amount of fruits!\n");
    //     return money;
    // }

    if (money < 100 * amount)
    {
        printf("You don't have enough money to buy fruit\n");
        return money;
    }

    return money - 100 * amount;
}

int buy_json(money)
{
    if (money < 100000000)
    {
        printf("You don't have enough money to buy json\n");
        return money;
    }

    printf("Yah you have bought json!\n");
    return money - 100000000;
}

int main()
{

    int money = 1000;
    int operation = 0;
    int leave = 0;

    while (!leave)
    {
        printf("You have %d dollars\n", money);
        printf("Enter your operation:\n");
        printf("1: Buy Fruit (100 dollars)\n");
        printf("2: Buy 郭哲文 (100000000 dollars)\n");
        printf("0: Leave\n");
        scanf("%d", &operation);

        switch (operation)
        {
        case 1:
            money = buy_fruit(money);
            break;
        case 2:
            money = buy_json(money);
            break;
        case 0:
            leave = 1;
            break;
        default:
            break;
        }
    }
}

// hw7: poker-21 game
// - Specify how many set of Poker to be used/played
// - Specify how many players
// - Show out result of game

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct card
{
    char *face;
    char *suit;
};
typedef struct card Card;

struct poker21
{
    int card_n; // Card number
    int sum;    // Total cards' credit
    Card sub[6];
};
typedef struct poker21 Poker21;

void fillDeck_n(Card *, char *[], char *[], int);
void shuffle_n(Card *, int);
void deal_Poker21(Card *, Poker21 *, int, int, int);
void show_player(Poker21 *, int);

char *FACE[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K"};
char *SUIT[] = {"H", "D", "C", "S"};

int main(void)
{

    int n;        // # of card sets
    int player_n; // # of players

    // 1. Initialization
    printf("Please specify how may set fo cards to play: ");
    scanf("%d", &n);

    printf("Please specify how many players for this Poker-21: ");
    scanf("%d", &player_n);

    printf("\n========================================================\n");
    printf("|\t Welcome to the Poker-21 game!\t\t\t|\n");
    printf("| You have used %d-set Poker cards.\t\t\t|\n", n);
    printf("| There are %d players.\t\t\t\t\t|\n", player_n);
    printf("========================================================\n\n");

    Card *deck = malloc(52 * n * sizeof(Card));
    Poker21 *player = malloc((player_n + 1) * sizeof(Poker21));

    for (int i = 0; i <= player_n; i++)
    {
        player[i].card_n = 0;
        player[i].sum = 0;
    }

    // 2. Fill n-set of Poker cards & shuffle cards.
    srand(time(NULL));
    fillDeck_n(deck, FACE, SUIT, n);
    shuffle_n(deck, n);

    // 3. Deal 1st-round cards (2 cards for the Banker & eacher Player)
    printf("======================== 1st ROUND =====================\n");
    int deal_begin = 0;

    for (int i = 0; i <= player_n; i++)
    {
        deal_Poker21(deck, &player[i], n, deal_begin, 2);
        deal_begin += 2;
        show_player(&player[i], i);
    }

    // 4. Deal 2nd-round cards for each player
    char YN;
    printf("======================== 2nd ROUND =====================\n");
    for (int i = 1; i <= player_n; i++)
    {
        show_player(&player[i], i);
        printf("Player %d, do you like to add another card (Y/N): ", i);
        scanf(" %c", &YN);

        while (YN == 'Y' | YN == 'y')
        {
            deal_Poker21(deck, &player[i], n, deal_begin, 1);
            deal_begin++;

            show_player(&player[i], i);
            printf("Player %d, do you like to add another card (Y/N): ", i);
            scanf(" %c", &YN);
        }
    }

    // 5. Deal 2nd-round cards for the Banker
    show_player(&player[0], 0);
    while (player[0].sum < 16)
    {
        deal_Poker21(deck, &player[0], n, deal_begin, 1);
        deal_begin += 1;

        show_player(&player[0], 0);
    }

    // 6. Get the result of the game
    for (int i = 1; i <= player_n; i++)
    {
        if ((player[i].sum > player[0].sum & player[i].sum <= 21) | (player[i].sum <= 21 & player[0].sum > 21))
        {
            printf("Player #%d WON!\n", i);
        }
        else if ((player[i].sum == player[0].sum) | (player[i].sum > 21 & player[0].sum > 21))
        {
            printf("Player #%d is SAFE!\n", i);
        }
        else
        {
            printf("Player #%d LOST!\n", i);
        }
    }

    return 0;
}

void fillDeck_n(Card *deck, char *face[], char *suit[], int n)
{
    for (int i = 0; i < 52 * n; i++)
    {
        deck[i].face = face[i % 13];
        int j = ((int)(i / 13)) % 4;
        deck[i].suit = suit[j];
    }
    return;
}

// Shuffle all cards in deck[]
void shuffle_n(Card *deck, int n)
{

    // 52*n is not random enough, so I shuffled more time.
    for (int k = 0; k < 100; k++)
    {
        for (int i = 0; i < 52 * n; i++)
        {
            int j = rand() % (52 * n);
            Card temp = deck[i];
            deck[i] = deck[j];
            deck[j] = temp;
        }
    }

    return;
}
// Deal "m" cards to the player using n-set of cards from the "begin"th card
void deal_Poker21(Card *deck, Poker21 *one, int n, int begin, int m)
{
    if (begin + m >= 52 * n)
    {
        printf("ERROR:\t deal out all of the cards.\n");
        exit(0);
    }

    int j = one->card_n;
    for (int i = 0; i < m; i++)
    {
        one->sub[i + j] = deck[begin + i];
    }
    one->card_n += m;
}

// Calculate cards' credit
int credit(Card *oneCard)
{

    int i = 1, result = 0;

    while (i <= 13 & result == 0)
    {
        if (i <= 9 & strcmp(oneCard->face, FACE[i - 1]) == 0)
            result = i;
        if (i >= 10 & strcmp(oneCard->face, FACE[i - 1]) == 0)
            result = 10;
        i += 1;
    }

    return result;
}

// Show the player's cards & total credit
void show_player(Poker21 *one, int p)
{

    // Top of the card
    printf("\n\t\t");
    for (int i = 0; i < one->card_n; i++)
    {
        printf(" =====  ");
    }

    // Number of the card
    printf("\n\t\t");
    for (int i = 0; i < one->card_n; i++)
    {
        printf("|%s    | ", one->sub[i].face);
    }

    // Discription
    printf("\nThe Player#%d :\t", p);
    one->sum = 0;
    for (int i = 0; i < one->card_n; i++)
    {
        printf("|  ");

        switch (*(one->sub[i].suit))
        {
        case 'D':
            printf("\xe2\x97\x86");
            break;
        case 'S':
            printf("\xe2\x99\xa0");
            break;
        case 'C':
            printf("\xe2\x99\xa3");
            break;
        case 'H':
            printf("\xe2\x99\xa5");
            break;
        }
        printf("  | ");
        one->sum += credit(&(one->sub[i]));
    }
    printf("==> %d", one->sum);
    printf("\n\t\t");

    // Buttom of the card
    for (int i = 0; i < one->card_n; i++)
    {
        printf("|    %s| ", one->sub[i].face);
    }
    printf("\n\t\t");
    for (int i = 0; i < one->card_n; i++)
    {
        printf(" =====  ");
    }
    printf("\n\n");
    return;
}
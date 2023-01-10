// - Specify how many set of Poker to be used/played
// - Specify how many players
// - Show out result of game

#include "poker.h"

static char *FACE[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K"};
static char *SUIT[] = {"H", "D", "C", "S"};

void print_rule(void){
    printf("\n\033[37m============================================================================================================\033[0m\n");
    printf("\033[37m|\033[0m     21點是一種經典的撲克牌遊戲，每局的目的是讓手中的牌點數加起來等於或接近21點，但不能超過21點。         \033[37m|\033[0m\n");
    printf("\033[37m|\033[0m     遊戲開始後，玩家先行動，可以選擇要牌或停牌。若玩家選擇要牌，莊家就會從牌堆中給玩家一張牌             \033[37m|\033[0m\n");
    printf("\033[37m|\033[0m     ，玩家的牌點數會隨之增加。玩家可以一直要牌直到他們的牌點數等於或超過21點，此時玩家爆牌了，失去本局。 \033[37m|\033[0m\n");
    printf("\033[37m|\033[0m     最後，玩家和莊家的牌點數會被比較，如果玩家的牌點數比莊家高，玩家贏得本局；                           \033[37m|\033[0m\n");
    printf("\033[37m|\033[0m     如果玩家的牌點數等於莊家的牌點數，則是和局；否則莊家贏。                                             \033[37m|\033[0m\n");
    printf("\033[37m============================================================================================================\033[0m\n");
}


int main(void)
{

    int n;        // # of card sets
    int player_n; // # of players
    int betting_odds_over21, betting_odds_under21; // betting ratio
    print_rule();
    
    // 1. Initialization
    printf("Please specify how may set fo cards to play: ");
    scanf("%d", &n);

    printf("Please specify how many players for this Poker-21: ");
    scanf("%d", &player_n);

    printf("Please enter the betting odds under 21: ");
    scanf("%d", &betting_odds_under21);

    printf("Please enter the betting odds over 21: ");
    scanf("%d", &betting_odds_over21);

    printf("\n=========================================================================\n");
    printf("|\t\t\t Welcome to the Poker-21 game!\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t|\n");
    printf("| You have used %d-set Poker cards.\t\t\t\t\t|\n", n);
    printf("| There are %d players.\t\t\t\t\t\t\t|\n", player_n);
    printf("| Betting odds under 21 is %d.\t\t\t\t\t\t\t|\n", betting_odds_under21);
    printf("| Betting odds over 21 is %d.\t\t\t\t\t\t\t|\n", betting_odds_over21);
    printf("=========================================================================\n\n");

    Card *deck = malloc(52 * n * sizeof(Card));
    Poker21 *player = malloc((player_n + 1) * sizeof(Poker21));

    for (int i = 0; i <= player_n; i++)
    {
        player[i].card_n = 0;
        player[i].sum = 0;
        printf("Player #%d, Please enter how much money you want to bet: ",i);
        scanf("%d", &(player[i].money));
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

            if (*(&player[i].sum) > 21)
            {
                printf("Player#%d's point total exceeds 21 points! \n",i);
                break;
            }
            
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "players_data.h"


typedef struct PlayerNode{
    int id;
    char name[50];
    int role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float perfIndex;
    struct PlayerNode *next;
} PlayerNode;

typedef struct team{
    char name[50];
    int id;
    int totalPlayers;
    float avgBattingSR;
    PlayerNode *rolehead[4];
} team;

int getRoleCode(const char *role){
    if(strcmp(role,"Batsman")==0){
        return 1;
    }
    if(strcmp(role,"Bowler")==0){
        return 2;
    }
    
        return 3;
}

const char *roleName(int r){
    if(r==1){
        return "Batsman";
    }
    if(r==2){
        return "Bowler";
    }
    return "All-rounder";
}


float computePerformanceIndex(PlayerNode *p){
    if(p->role==1){
        return (p->battingAverage*p->strikeRate)/100.0;
    } else if(p->role==2){
          return (p->wickets * 2) + (100.0 - p->economyRate);
    } else {
             return ((p->battingAverage * p->strikeRate) / 100.0) + (p->wickets * 2);
             }
}

int findTeamIndex(const char *name)
{
    for (int i = 0; i < teamCount; i++)
    {
        if (strcmp(teams[i], name) == 0)
            return i;
    }
    return -1;
}

void insertPlayerSorted(team *team, PlayerNode *p){
    int role = p->role;
    PlayerNode **head = &team->rolehead[role];
    PlayerNode *curr = *head;

    if(*head==NULL || p->perfIndex> curr->perfIndex){
        p->next = *head;
        *head=p;
    } else{
        while(curr->next && curr->next->perfIndex >= p->perfIndex){
            curr=curr->next;
        }
        p->next = curr->next;
        curr->next=p;
    }

    team->totalPlayers++;
    if(role!=2){
        float total = team->avgBattingSR * (team->totalPlayers - 1);
        team->avgBattingSR = (total + p->strikeRate) / team->totalPlayers;
    }

}


void initializeTeams(team teamList[]){

    for(int i=0;i<teamCount;i++){
        strcpy(teamList[i].name,teams[i]);
        teamList[i].id = i+1;
        teamList[i].totalPlayers=0;
        teamList[i].avgBattingSR=0;
        for(int j=0;j<=3;j++){
            teamList[i].rolehead[j]=NULL;
        }
    }
}

void loadPlayers(team teamList[]){
    for(int i=0;i<playerCount;i++){
        PlayerNode *p= malloc(sizeof(PlayerNode));
        if (p == NULL)
        {
            printf("error in memory allocation try again!");
            exit(1);
        }

        p->id = players[i].id;
        strcpy(p->name, players[i].name);
        p->role = getRoleCode(players[i].role);
        p->totalRuns = players[i].totalRuns;
        p->battingAverage = players[i].battingAverage;
        p->strikeRate = players[i].strikeRate;
        p->wickets = players[i].wickets;
        p->economyRate = players[i].economyRate;
        p->perfIndex = computePerformanceIndex(p);
        p->next = NULL;

        int index = findTeamIndex(players[i].team);
        insertPlayerSorted(&teamList[index],p);

    }
}

void displayPlayersOfTeam(team teamList[]){
    char teamName[50];
     printf("Enter Team Name: ");
    scanf(" %[^\n]", teamName);

    int idx = findTeamIndex(teamName);
     if (idx == -1)
    {
        printf("Team not found!\n");
        return;
    }

     team *t = &teamList[idx];

     printf("Players of %s\n", t->name);
      printf("%-5s %-25s %-12s %-10s %-10s %-10s\n",
           "ID", "Name", "Role", "Runs", "Avg", "SR");
    printf("--------------------------------------------------------------------\n");

    for(int role=1;role<=3;role++){
        PlayerNode *p = t->rolehead[role];
         while (p)
        {
            printf("%-5d %-25s %-12s %-10d %-10.2f %-10.2f\n",
            p->id, p->name, roleName(role),
            p->totalRuns, p->battingAverage, p->strikeRate);
            p = p->next;
        }
    }

}


int cmpTeams(const void *a, const void *b)
{
    float x = ((team *)a)->avgBattingSR;
    float y = ((team *)b)->avgBattingSR;
    return (y > x) - (y < x);
}


void displayTeamSorted(team teamList[]){
    team arr[10];
    for(int i=0;i<10;i++){
        arr[i] = teamList[i];
    }

    qsort(arr,10,sizeof(team),cmpTeams);
    printf("\n%-15s %-20s %-10s\n", "Team", "Avg Batting SR", "Players");
    printf("-----------------------------------------------------\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%-15s %-20.2f %-10d\n",
               arr[i].name, arr[i].avgBattingSR, arr[i].totalPlayers);
    }
}

void topKPlayer(team teamList[]){
    char name[50];
    int role,K;

    printf("Enter Team Name: ");
    scanf(" %[^\n]", name);
    printf("Enter Role in number (1=Batsman 2=Bowler 3=All-rounder): ");
    scanf("%d", &role);
    printf("Enter K: ");
    scanf("%d", &K);

    int idx = findTeamIndex(name);
    if(idx==-1){
        printf("please enter a valid team!/n");
        return;
    }

    PlayerNode *p = teamList[idx].rolehead[role];

    if (!p)
    {
        printf("No players of this role.\n");
        return;
    }

    printf("Top %d %s from %s:\n", K, roleName(role), name);
    printf("%-5s %-20s %-10s\n", "ID", "Name", "PerfIndex");
    printf("-----------------------------------\n");

    while (p && K--)
    {
        printf("%-5d %-20s %-10.2f\n", p->id, p->name, p->perfIndex);
        p = p->next;
    }
}


void allPlayersByRole(team teamList[])
{
    int role;
    printf("Enter Role (1=Batsman 2=Bowler 3=All-rounder): ");
    scanf("%d", &role);

    PlayerNode *arr[10];
    for (int i = 0; i < 10; i++)
        arr[i] = teamList[i].rolehead[role];

    printf("\nAll %s across teams (sorted):\n", roleName(role));

    while (1)
    {
        int best = -1;
        float bestPerf = -1;

        for (int i = 0; i < 10; i++)
        {
            if (arr[i] && arr[i]->perfIndex > bestPerf)
            {
                bestPerf = arr[i]->perfIndex;
                best = i;
            }
        }

        if (best == -1)
            break;

        PlayerNode *p = arr[best];
        printf("%-5d %-20s %-12s %-10.2f\n",
               p->id, p->name, teamList[best].name, p->perfIndex);

        arr[best] = arr[best]->next;
    }
}


void addPlayer(team teamList[])
{
    PlayerNode *p = malloc(sizeof(PlayerNode));
    if (!p)
    {
        printf("error in memory allocation try again!");
        exit(1);
    }

    char teamName[50], roleStr[20];

    printf("Enter new Player ID: ");
    scanf("%d", &p->id);

    printf("Enter Player Name: ");
    scanf(" %[^\n]", p->name);

    printf("Enter Team Name: ");
    scanf(" %[^\n]", teamName);

    printf("Enter Role (Batsman/Bowler/All-rounder): ");
    scanf(" %[^\n]", roleStr);

    p->role = getRoleCode(roleStr);

    printf("Total Runs: ");
    scanf("%d", &p->totalRuns);
    printf("Batting Average: ");
    scanf("%f", &p->battingAverage);
    printf("Strike Rate: ");
    scanf("%f", &p->strikeRate);
    printf("Wickets: ");
    scanf("%d", &p->wickets);
    printf("Economy Rate: ");
    scanf("%f", &p->economyRate);

    p->perfIndex = computePerformanceIndex(p);
    p->next = NULL;

    int idx = findTeamIndex(teamName);
    if (idx == -1)
    {
        printf("Team not found!\n");
        free(p);
        return;
    }

    insertPlayerSorted(&teamList[idx], p);

    printf("Player added successfully!\n");
}

void showMenu()
{
    printf("\n=================== MENU ================\n");
    printf("1. Add Player\n");
    printf("2. Display Players of a Team\n");
    printf("3. Display Teams Sorted by Avg SR\n");
    printf("4. Top K Players of a Team by Role\n");
    printf("5. Display All Players by Role Across Teams\n");
    printf("6. Exit\n");
    printf("=========================================\n");
    printf("Enter choice: ");
}


int main(){
    team teamList[10];

    initializeTeams(teamList);
    loadPlayers(teamList);

    printf("Initialization Complete loaded %d players.", playerCount);

    int choice;
    while(1){
        showMenu();
        scanf("%d",&choice);

        switch (choice)
        {
        case 1:
            addPlayer(teamList);
            break;
        case 2:
            displayPlayersOfTeam(teamList);
            break;
        case 3:
            displayTeamSorted(teamList);
            break;
        case 4:
            topKPlayer(teamList);
            break;
        case 5:
            allPlayersByRole(teamList);
            break;
        case 6:
            printf("Goodbye....\n");
            return 0;
        default:
            printf("Invalid choice!\n");
        
        }
    }
}
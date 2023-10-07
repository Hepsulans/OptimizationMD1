#include <iostream>
#include <windows.h>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

void StartingPos(int DataX[][50], int X_Hor, int X_Ver, int Grid[][100]) // int PasDataY[], int Y_Hor, int Y_Ver - For the starting position we don't need the DataY as it is only needed to calculate the Correction value.
{
    for(int i=0; i<X_Hor; i++)
    {
        int offset = 0; // The offset for black square filling starting position;
        for(int j=0; j<X_Ver; j++)
        {
            //cout << "CurrentCordinate: " << i << " " << j << endl;
            int BlackSq = DataX[i][j]; // Passes the amount of black squares needed to be added to the grid.
            //cout << BlackSq << endl;
            int compOffset = 0;
            for(int k=0+offset; k<BlackSq+offset; k++) // Fills the passed amount of black squares on the grid.
            {
                //cout << "Black here: " << i << " " << k << endl;
                Grid[i][k] = 1; // Makes the position Black.
                //cout << "Grid gained: "<< Grid[i][k] << endl;

                compOffset++; // Increases the offset by 1. Because the game requires at least 1 white square in between black ones.
            }
            offset = offset + compOffset + 1;
        }
    }
}

int CostValue(int Grid[][100], int Y_Cost[50], int Y_Cost_Instance[50], int X_Hor, int  Y_Ver)
{
    int CurrentFValue = 0;
    // Checking for row value correctness // Currently an incorrect row gives a +1 for every incorrect amount.
    for(int a = 0; a<Y_Ver; a++)
    {
        int RowValue = 0;
        for(int b = 0; b<X_Hor; b++)
        {
            RowValue = RowValue + Grid[b][a];
        }
        //cout << Y_Cost[a] << " " << RowValue << endl;
        if(Y_Cost[a] - RowValue <0){ // Module type of shenanigans.
            CurrentFValue = CurrentFValue + (-1*(Y_Cost[a] - RowValue));
        }else{
            CurrentFValue = CurrentFValue + Y_Cost[a] - RowValue;
        }
    }

    // Checking for instance correctness // Currently an incorrect instance gives a +1 for every incorrect amount.
    for(int a = 0; a<Y_Ver; a++)
    {
        int InstanceValue = 0;
        int inFound = 0;
        for(int b = 0; b<X_Hor; b++)
        {
            if(Grid[b][a] == 1 && inFound != 1) // Instance found
            {
                inFound++;
                InstanceValue++;
            } else if (Grid[b][a] == 0){ // 0 found. Instance ends. Reset the inFound.
                inFound = 0;
            }
        }
        if(Y_Cost_Instance[a] - InstanceValue <0){ // Module type of shenanigans.
            CurrentFValue = CurrentFValue + (-1*(Y_Cost_Instance[a] - InstanceValue));
        }else{
            CurrentFValue = CurrentFValue + Y_Cost_Instance[a] - InstanceValue;
        }
    }

    return CurrentFValue;
}

int CostValue_Mega(int PotGrid[100][100][2], int Cur_Grid_Inspection, int Y_Cost[50],int Y_Cost_Instance[50], int X_Hor, int  Y_Ver)
{
    int CurrentFValue = 0;
    for(int a = 0; a<Y_Ver; a++)
    {
        int RowValue = 0;
        for(int b = 0; b<X_Hor; b++)
        {
            RowValue = RowValue + PotGrid[b][a][Cur_Grid_Inspection];
        }
        //cout << Y_Cost[a] << " " << RowValue << endl;
        if(Y_Cost[a] - RowValue <0){ // Module type of shenanigans.
            CurrentFValue = CurrentFValue + (-1*(Y_Cost[a] - RowValue));
        }else{
            CurrentFValue = CurrentFValue + Y_Cost[a] - RowValue;
        }
    }

    for(int a = 0; a<Y_Ver; a++)
    {
        int InstanceValue = 0;
        int inFound = 0;
        for(int b = 0; b<X_Hor; b++)
        {
            if(PotGrid[b][a][Cur_Grid_Inspection] == 1 && inFound != 1) // Instance found
            {
                inFound++;
                InstanceValue++;
            } else if (PotGrid[b][a][Cur_Grid_Inspection] == 0){ // 0 found. Instance ends. Reset the inFound.
                inFound = 0;
            }
        }
        if(Y_Cost_Instance[a] - InstanceValue <0){ // Module type of shenanigans.
            CurrentFValue = CurrentFValue + (-1*(Y_Cost_Instance[a] - InstanceValue));
        }else{
            CurrentFValue = CurrentFValue + Y_Cost_Instance[a] - InstanceValue;
        }
    }

    return CurrentFValue;
}

void EqualizePotGrid(int PotGrid[100][100][2],int Grid[100][100],int X_Hor, int Y_Ver)
{
    for(int b = 0; b<X_Hor; b++)
    {
        for(int c = 0; c<Y_Ver; c++)
        {
            for(int a = 0; a<2; a++)
            {
                PotGrid[b][c][a] = Grid[b][c];
                //cout << Grid[b][c];
            }
        }
        //cout << endl;
    }
}

// Experimental grid was better than the best grid (At least according to the costFunction)
void NewBestGrid(int PotGrid[100][100][2],int X_Hor, int Y_Ver)
{
    for(int b = 0; b<X_Hor; b++)
    {
        for(int c = 0; c<Y_Ver; c++)
        {
            PotGrid[b][c][1] = PotGrid[b][c][0];
        }
        //cout << endl;
    }
}

// Best known grid replaces the experimental one
void SameGrid (int PotGrid[100][100][2],int X_Hor, int Y_Ver)
{
    for(int b = 0; b<X_Hor; b++)
    {
        for(int c = 0; c<Y_Ver; c++)
        {
            PotGrid[b][c][0] = PotGrid[b][c][1];
        }
        //cout << endl;
    }
}

void buildBlockSelection (int DataX[50][50],int X_Hor,int X_Ver,int GridBlocks[50])
{
    for(int a = 0; a < X_Hor; a++)
    {
        for(int b = 0; b < X_Ver; b++)
        {
            if(DataX[a][b] >= 1){
                GridBlocks[a]++;
            }
        }
        // cout << "Found blocks: "<< GridBlocks[a] << endl;
    }
}

void UpsideDownGrid(int Grid[100][100],int X_Hor,int Y_Ver)
{
    int* new_Order = new int[Y_Ver]; // The values in a correct way;

    for(int i = 0; i < X_Hor; i++) // Inverted so that the image starts from the bottom. Its easier for me to grasp that way
    {
        for(int j = 0; j < Y_Ver; j++)
        {
            new_Order[Y_Ver-j-1] = Grid[i][j]; // Writing data reversed on new array
            //cout << Y_Ver-j-1 << " | ";
            //cout << Grid[i][j] << " | ";
        }

        for(int j = 0; j < Y_Ver; j++)
        {
            //cout << new_Order[j] << " | ";
            Grid[i][j] = new_Order[j]; // Writing data reversed on the current array
        }
        //cout << endl;
    }

    delete[] new_Order; // Clear memory
}

// GridBlocks - Black block cluster amount in each column | PotGrid - Grids that we are going to manipulate till we find the most optimal outcome. And hopefully the correct outcome to the game.
// IzmVert - costFunction for each of the PotGrid options so far. // Izmaksu veertibas
// X_Hor - Horizontal grid size | Y_Ver - Vertical grid size
void Optimizer_TheGameGrid (int GridBlocks[50],int PotGrid[100][100][2],int IzmVert[10],int X_Hor,int Y_Ver,int Y_Cost[50],int Y_Cost_Instance[50])
{
    //cout << "We are here!" << endl;

    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    /*cout << "Optimizer_TheGameGrid" << endl;
    cout << "____" << endl;
    for(int i = 0; i < Y_Ver; i++) // Inverted so that the image starts from the bottom. Its easier for me to grasp that way
    {
        for(int j = 0; j < X_Hor; j++)
        {

            if(PotGrid[j][i][0] == 1)
            {
                SetConsoleTextAttribute(hConsole, 14);
                cout << PotGrid[j][i][0];
            } else {
                SetConsoleTextAttribute(hConsole, 1);
                cout << PotGrid[j][i][0];
            }
        }
        cout << endl;
    }
    SetConsoleTextAttribute(hConsole, 7); // Returns the color to default
    cout << "____" << endl;*/

    // How the random function worked:
    // srand (time(NULL)); // Initialize random time seed
    //int iSecret = rand() % 10 + 1;
    //cout << "Testing num gen from 1-10:" << iSecret << endl;

    srand (time(NULL)); // Initialize random time seed

    //int* Viable_Pos = new int[Y_Ver]; // Creation of a dynamic array that contains info on which positions will be usable for potential relocations of black blocks
    //int* Current_Pos = new int[Y_Ver]; // Creates a dynamic array that will be used to remember the current position of our Black Block.

    /// PotGrid got redefined because of my understanding of the LAHC requirements
    /// PotGrid[?][?][0] // Experimental grid.
    /// PotGrid[?][?][1] // Known Best grid.

    int BB_Start = -1, BB_End = -1, RB_Start = -1, RB_End = -1; // Variables for the BB starting and ending positions in the array. || Variables for the Relocation positions of BB Start and End.
    int Cur_Check = -1; // Representation of which costFunction is being operated on currently. // Start off with -1 so that we can adjust the value at the beginning of the do/while cycle.
    int Safe_Guard = 0; // If reaches a value of 2 it means that the BB is not relocatable and a new one needs to be selected.
    int LAHC = 0; // Value of the Late Acceptance Hill-Climbing cost function evaluation variable.

    // Relocated variables because they became untouchable because of the do/while with the safeguard
    int row_Pos; // We need the 0 to be included so no +1. This will obtain a number between 0-4
    int target_BCluster; // Find out which clusters of Black Blocks we are going to move in this column.
    int C_Insp_Clust; // The currently found black box cluster.

    int EC_UP, EC_DOWN; // Values to control edge cases so the code does not do things that are not intended.
    int a_Controller; // Controls whatever "a" was increased by the for cycle or not

    int Current_Best_CFunction=0; // Just a little something to make sure you know that the optimizer is not just existing. And that its working.

    do{
            /*
        cout << "Grid that is being manipulated right now:" << endl;
        cout << "____" << endl;
        for(int i = 0; i < Y_Ver; i++) // Inverted so that the image starts from the bottom. Its easier for me to grasp that way
        {
            for(int j = 0; j < X_Hor; j++)
            {

                if(PotGrid[j][i][0] == 1)
                {
                    SetConsoleTextAttribute(hConsole, 14);
                    cout << PotGrid[j][i][0];
                } else {
                    SetConsoleTextAttribute(hConsole, 1);
                    cout << PotGrid[j][i][0];
                }
            }
            cout << endl;
        }
        SetConsoleTextAttribute(hConsole, 7); // Returns the color to default
        cout << "____" << endl;
            */



        // Rotation between PotGrids
        // THE VALUE OF (Cur_Check==3) DETERMINATES HOW MANY INSTANCES YOU ALLOW THE OPTIMIZER TO REMEBER.
        if(Cur_Check==10)
        {
            Cur_Check = 0;
        } else {
            Cur_Check++;
        }

        do{ // The safeguard check
        BB_Start = -1, BB_End = -1; // In case the Safe_Guard triggered the BB values need to be deleted as new ones will be found.
        Safe_Guard = 0; // SafeGuard gets reset back to 0.
        row_Pos = rand() % X_Hor; // We don't need the 0 to be included so no +1. This will obtain a number between 0-4
        //cout << "X_Hor: " << X_Hor <<endl;
        target_BCluster = rand() % GridBlocks[row_Pos] + 1; // Find out which clusters of Black Blocks we are going to move in this column. // Needs a +1 because C_Insp_Clust operates from 1+
        C_Insp_Clust = 0; // The currently found black box cluster.
        EC_UP = 0; // Edge case reset.
        EC_DOWN = 0; // Edge case reset.
        a_Controller = 0; // a_Controller reset.

        //cout << "GridBlocks[row_Pos]: "<<GridBlocks[row_Pos] << endl;

        // Hunting for the BB position. Its Start and End
        for(int a = 0; a < Y_Ver; a++) // Goes through the whole column to find the target / to find the valid locations.
        {
            a_Controller++; // a_Controller was just manipulated by the FOR cycle

            //cout << "a++ | post for" << "PotGrid[row_Pos][a][0]: " << PotGrid[row_Pos][a][0] << endl;
            if(PotGrid[row_Pos][a][0] == 1) // With the FOR cycle and this we can find the correct target. // THIS CHECK MIGHT BE OBSOLETE
            {
                //cout << "(PotGrid[row_Pos][a][0]: " <<PotGrid[row_Pos][a][0] << " | row_Pos:" << row_Pos << " | a:" << a << endl;
                C_Insp_Clust++; // New block found we notify the variable of it.
                //cout << "C_Insp_Clust: " << C_Insp_Clust << " | target_BCluster:" << target_BCluster << endl;
                do{
                    if(a_Controller == 0){ // Only increase a if it hasnt been increase by the FOR cycle right before.
                        a++;
                        a_Controller = 0; // Reset the a_Controller as the last increasing object was this place. Hence we can continue using it till the FOR cycle gets used.
                    }

                    if(C_Insp_Clust == target_BCluster) // Check if its the correct Black Block (this is inefficient as we are checking it every do cycle... but oh well)
                    {
                        // We are at the first BB. EDGE CASE UPPER
                        if(a==0)
                        {
                            //cout << "1 a: " << a << endl;
                            BB_Start = a; // Then we have found the BB start.
                            //cout << "BB_Start: " << BB_Start << endl;
                            Safe_Guard++; // There is no where to move upwards.
                            EC_UP = 1; // Edge case upper. Do not read upwards!!!
                        }

                        if(PotGrid[row_Pos][a-1][0] == 0 && EC_UP != 1) // (Changed) SHOULD IT NOT LOOK FOR THE PREVIOUS BEEING A 0??? Otherwise we are constantly changing the start while looking for BB_End
                        {
                            //cout << "2 a: " << a << endl;
                            BB_Start = a; // Then we have found the BB start.
                            //cout << "BB_Start: " << BB_Start << endl;
                            if(PotGrid[row_Pos][a-2][0] == 1)
                            {
                                //cout << "2 Safe_Guard" << endl;
                                Safe_Guard++; // There is no where to move upwards.
                            }
                        }

                        // We are at the last BB. EDGE CASE LOWER
                        if(a == (Y_Ver-1)) // Case 001
                        {
                            //cout << "3 a: " << a << endl;
                            BB_End = a; // Then we have found the BB end.
                            //cout << "BB_End: " << BB_End << endl;
                            Safe_Guard++; // There is no where to move lower.
                            EC_DOWN = 1; // Edge case lower. Do not read lower!!!
                            a = Y_Ver; // Quite sure we don't need to do this one but just in case.
                        } else if (a == (Y_Ver-2) && PotGrid[row_Pos][a+1][0] == 1){ // This does not really need to be an else if... But it is somehow easier for me to understand. | Case 011
                            //cout << "4 a: " << a << endl;
                            BB_End = Y_Ver-1; // Then we have found the BB end. BB_End is the bottom.
                            //cout << "BB_End: " << BB_End << endl;
                            Safe_Guard++; // There is no where to move lower.
                            EC_DOWN = 1; // Edge case lower. Do not read lower!!!
                            a = Y_Ver; // Quite sure we don't need to do this one but just in case.
                        } else if (a == (Y_Ver-2)){ // Case 010
                            //cout << "6 a: " << a << endl;
                            BB_End = a; // Then we have found the BB end.
                            //cout << "BB_End: " << BB_End << endl;
                            // Safe_Guard++; // There Is 1 block lower. After all this is case 010
                            EC_DOWN = 1; // Edge case lower. Do not read lower!!!
                            a = Y_Ver; // Quite sure we don't need to do this one but just in case.
                        }

                        if(PotGrid[row_Pos][a+1][0] != 1 && EC_DOWN != 1) // If the next one is not a 1
                        {
                            //cout << "5 a: " << a << endl;
                            BB_End = a; // Then we have found the BB end.
                            //cout << "BB_End: " << BB_End << endl;
                            a = Y_Ver; // We have found our target and we need to leave this locations and the semi-global For cycle.
                            if(PotGrid[row_Pos][a+2][0] == 1)
                            {
                                //cout << "5 Safe_Guard" << endl;
                                Safe_Guard++; // There is no where to move downwards.
                            }
                        }
                    }

                    //cout << "a++ | before while: " << a << " | Y_Ver: " << Y_Ver << endl;

                    //a++; //     THIS

                    //cout << PotGrid[row_Pos][a][0] << " != " << 1 << " && " << a << " < " << Y_Ver << endl;
                }while(PotGrid[row_Pos][a][0] != 1 && a < Y_Ver); // in a do while it needs to be an && if we want any of the situation being false allowing to leave.
                //cout << "We left the while" << endl;
            } // if found a 0 just ignore...
            // cout << "101" << endl; // THERE ARE ERRORS IN THESES PARTS OF THE CODE
            //cout << "row_Pos:" << row_Pos << " | target_BCluster:" << target_BCluster << "  | C_Insp_Clust:" << C_Insp_Clust << "  | Safe_Guard:" << Safe_Guard << endl;
            //cout << "EC_UP:" << EC_UP << " | EC_DOWN:" << EC_DOWN << endl;
            //cout << "BB values: " << BB_Start << "|" << BB_End << endl;
        }
        }while(Safe_Guard == 2);

        //cout << "------" << endl;
        //cout << "After the safeGuard Do/While" << endl;

        // Locating potential relocation positions of BB // (Done) -> EDGE CASES ARE NOT BEEING CONSIDERED
        for(int a = BB_Start; a >= 0; a--) // For example a = 3
        {
            RB_Start = a; // Found the start of the relocation zone.

            if(a == 0) // Case 1000 | 101101
            {
                RB_Start = a; // Relocation can start from the top. // And the exit clause is active already.
            }else if(a == 1) // Case 01011 | 011001
            {
                a = 0; // Exit clause
                RB_Start = a; // Relocation can start from the top.
            }else if(PotGrid[row_Pos][a-2][0] != 1 && (a-2) == 0) // The end is a 0 // EDGE CASE. 2 reading above is the end. // Works in cases where there are 000..01 in the beginning
            {
                a = 0; // Exit clause
                RB_Start = a; // Relocation can start from the top.
            }else if(PotGrid[row_Pos][a-2][0] != 0) // Checking 2 blocks further as we need to have 1 buffer 0. // Works in cases where there is a 1 two blocks above.
            {
                a = 0; // We have found the RB_Start and need to exit this For loop. // This needs to exit the loop if not CHANGE IT!
            }
        }

        for(int a = BB_End; a < Y_Ver; a++)
        {
            RB_End = a; // Found the end of the relocation zone.

            if((a+1) == Y_Ver || (a+2) == Y_Ver) // a+1 is for 0001 | a+2 is for 00010. Everything else is done with the ELSE IF
            {
                a = Y_Ver; // Exit clause
                RB_End = Y_Ver-1; // The bottom is the last place you can relocate. Y_Ver-1 = should be in bound.
            }else if(PotGrid[row_Pos][a+2][0] != 0) // Checking 2 blocks further as we need to have 1 buffer 0. // EDGE CASE. 2 reading below is out of bounds reading.
            {
                a = Y_Ver; // We have found the RB_End and need to exit this For loop. // This needs to exit the loop if not CHANGE IT!
            }
        }

        //cout << "RB_Start: " << RB_Start << " | RB_End: " << RB_End << endl;
        //cout << "Locating potential relocation positions of BB" << endl;

        // Obtain the viable starting position of the relocation. BB_Start = 3, BB_End = 4, RB_Start = 2, RB_End = 5;
        int BB_Size = BB_End - BB_Start + 1; // Obtain the size of the BB. BB_Size = 2 example
        int RA_Start = RB_Start + BB_Size - 1; // RelocationAdjusted_Start = RelocationBox_Start + BlackBox_Size - 1(For adjustment)
        int RA_End = RB_End; // RelocationAdjusted_End = RB_End. No need to do math for this one
        // RA_Start = 3 | RA_End = 5 example.

        //cout << "Obtain the viable starting position of the relocation" << endl;

        // Randomize the relocation starting position.

        int Atempt = 0; // How many atempts have been made so far.

        int Relocation_Begins_Here;
        do{
            Atempt++;
            Relocation_Begins_Here = rand() % (RA_End-RA_Start+1) + RA_Start;
        } while (Relocation_Begins_Here == BB_End && Atempt <= 2); // We keep randomizing the location till we chose one that is literally not the same as the current one
        // The SafeGuard should have made sure we did not get a situation with an object that cannot be moved
        // SEEMS LIKE THAT IS NOT THE CASE. OBJECTS ARE GETTING HERE WITH UNMOVABLE SITUATIONS.
            // 1. FIX THE SELECTION PROCESS
            // 2. ALLOW A SELECTION OF THE SAME POSITION AFTER 3 TRIES. [Did not work...] The BB start/end choices are a bit too fucked.
        //cout << "Relocation_Begins_Here: " << Relocation_Begins_Here << endl;
        //cout << "Randomize the relocation starting position." << endl;

        // Conducting the relocation move!
        // Delete the previous BB
        //cout << "Delete from BB_Start: " << BB_Start << " | Delete to BB_End+1: " << BB_End+1 << endl;
        for(int a = BB_Start; a < BB_End+1; a++) // BB_End + 1 because i need the end to be included. // Tested the +1 is not needed. it will include the end even if they are the same.
        {
            //cout << a << "|";
            PotGrid[row_Pos][a][0] = 0;
        }
        //cout << endl;

        // Create the new BB
        //cout << "Create from Relocation_Begins_Here: " << Relocation_Begins_Here << " | To Relocation_Begins_Here - BB_Size: " << Relocation_Begins_Here - BB_Size << endl;
        //for(int b = Relocation_Begins_Here + BB_Size; b > Relocation_Begins_Here; b--)
        for(int b = Relocation_Begins_Here; b > Relocation_Begins_Here - BB_Size; b--)
        {
            PotGrid[row_Pos][b][0] = 1;
        }

        //cout << "Conducting the relocation move!" << endl;

        // Recalculate the CostFunction of the currently inspected PotGrid.
        int TheFValue = CostValue_Mega(PotGrid,0,Y_Cost,Y_Cost_Instance,X_Hor,Y_Ver); // Obtained the value of the new grid.
        //cout << "The new grid has a F_Value of: " << TheFValue << endl;
        // Compares the obtain grids CostFunction value to the current rotations CostValue
        if(TheFValue >= IzmVert[Cur_Check])
        {
            //cout << "+++ WORSE OR SAME GRID! +++" << " | TheFValue (manipulated grid): " << TheFValue << " | IzmVert[Cur_Check] (Stored Grid): " << IzmVert[Cur_Check] << endl;
            // The new grids costFunction is the same or worse.
            IzmVert[Cur_Check] = TheFValue; // Add the costFunctions value to the rotation

            // Best grid overrides the current one.
            SameGrid(PotGrid,X_Hor,Y_Ver);

        } else if (TheFValue < IzmVert[Cur_Check])
        {
            //cout << "+++ BETTER GRID! +++" << " | TheFValue (manipulated grid): " << TheFValue << " | IzmVert[Cur_Check] (Stored Grid): " << IzmVert[Cur_Check] << endl;
            // The new grids costFunction is better than the rotations value! New starting grid is chosen!
            IzmVert[Cur_Check] = TheFValue; // Add the costFunctions value to the rotation

            // Current grid overrides the previously known best one
            NewBestGrid(PotGrid,X_Hor,Y_Ver);
        }

        //cout << "Relocation successful!" << endl;
        if((Current_Best_CFunction%1000000) == 0)
        {
            cout << "The last FValue was: " << TheFValue << endl;
        }
        Current_Best_CFunction++;

    }while(IzmVert[Cur_Check] != 0 && (Current_Best_CFunction < 100000000 && IzmVert[Cur_Check] > 2)); // Currently will keep trying to optimize till it finds a grid with a costFunction of 0.
    //cout << "We exited the do/while!" << endl;
}

int main()
{
    // The analysis grid
    int G_Size = 100; // For whatever reason if I do it like this. The function does not recognize it... // SHOULD PROBABLY USE DIFFERENT 2D ARRAY PASSSING METHOD...
    int Grid[100][100]; // For starters lets assume a grid max size of 100x100.
    // Filling the grid with the White blocks. Which is 0's in this representation.
    for (int i = 0; i < G_Size; i++)
    {
        for (int j = 0; j < G_Size; j++)
        {
            Grid[i][j] = 0;
        }
    }

    // Input data grid
    int DataX[50][50]; // Input data for the X-axis
    int DataY[50][50]; // Input data for the Y-axis
    int X_Hor, X_Ver, Y_Hor, Y_Ver;

    // Example data and also the way data is being read.
    // To see the picture this data represents check this link: https://www.nonograms.org/nonograms/i/66206
    /*
    Example data fill
    [X][Y] - Axis for myself
    DataX: // Going from Right to Left
    [0][0] = 1, [1][0] = 2, [2][0] = 5, [3][0] = 1, [4][0] = 1
    [0][1] = 1, [1][1] = 1, [2][1] = 1, [3][1] = 1, [4][1] = 0
    [0][2] = 0, [1][2] = 1, [2][2] = 0, [3][2] = 0, [4][2] = 0

    DataY: // Going from Top to Down
    [0][0] = 3, [0][1] = 1, [0][2] = 3, [0][3] = 1, [0][4] = 1, [0][3] = 2, [0][4] = 3
    [1][0] = 0, [1][1] = 1, [1][2] = 0, [1][3] = 0, [1][4] = 0, [1][3] = 0, [1][4] = 0
    */

    // Example data being inputed manually.
    X_Hor = 5;
    X_Ver = 3;
    // Input the data for X
    DataX[0][0] = 1; DataX[1][0] = 2; DataX[2][0] = 5; DataX[3][0] = 1; DataX[4][0] = 1;
    DataX[0][1] = 1; DataX[1][1] = 1; DataX[2][1] = 1; DataX[3][1] = 1; DataX[4][1] = 0;
    DataX[0][2] = 0; DataX[1][2] = 1; DataX[2][2] = 0; DataX[3][2] = 0; DataX[4][2] = 0;

    Y_Hor = 2;
    Y_Ver = 7;
    // Input the data for Y
    DataY[0][0] = 3; DataY[0][1] = 1; DataY[0][2] = 3; DataY[0][3] = 1; DataY[0][4] = 1; DataY[0][5] = 2; DataY[0][6] = 3;
    DataY[1][0] = 0; DataY[1][1] = 1; DataY[1][2] = 0; DataY[1][3] = 0; DataY[1][4] = 0; DataY[1][5] = 0; DataY[1][6] = 0;

    StartingPos(DataX,X_Hor,X_Ver,Grid); // Initializing the starting position.
    UpsideDownGrid(Grid,X_Hor,Y_Ver); // Turns the grid values upside down... because i have a feeling the issue is me doing this "Y_Ver-1" and other places just using the Y_Ver value.
    //cout << Grid[0][0] << endl;

    // Allows me to change the color of the text
    // Color variants come from these post:
    // https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c
    // https://www.daniweb.com/programming/software-development/code/216345/add-a-little-color-to-your-console-text
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


    // Output picture
    cout << "Starting Position:" << endl;
    cout << "____" << endl;
    for(int i = 0; i < Y_Ver; i++) // Inverted so that the image starts from the bottom. Its easier for me to grasp that way
    {
        for(int j = 0; j < X_Hor; j++)
        {

            if(Grid[j][i] == 1)
            {
                SetConsoleTextAttribute(hConsole, 14);
                cout << Grid[j][i];
            } else {
                SetConsoleTextAttribute(hConsole, 1);
                cout << Grid[j][i];
            }
        }
        cout << endl;
    }
    SetConsoleTextAttribute(hConsole, 7); // Returns the color to default
    cout << "____" << endl;

    int IzmVert[10]; // Izmaksu vertiba un visas parejas. Inicalizejam ar tam atminu
    int PotGrid[100][100][2]; // Potential grids and their value placements.
    int TempVal; // A value that gets changed whenever needed.

    int Y_Cost[50]; // Obtain the value of each Y row.
    int Y_Cost_Instance[50]; // Obtain the amount of instances in each Y row.
    for(int i = 0; i<Y_Ver; i++)
    {
        int CorrectLineValue = 0; // Value tracker
        int CorrectInstanceValue = 0; // Instance tracker
        for(int j = 0; j<Y_Hor; j++)
        {
            CorrectLineValue = CorrectLineValue + DataY[j][i];
            if(DataY[j][i] != 0)
            {
                CorrectInstanceValue++;
            }
            //cout << CorrectInstanceValue << " ";
            //cout << DataY[j][i] << " ";
        }
        Y_Cost[i] = CorrectLineValue; // grids are flipped no need to invert // Needed to write these values in an inverted form. Because the grids are written that way (I think that was fixed...)
        Y_Cost_Instance[i] = CorrectInstanceValue;
        //cout << endl;
        //cout << CorrectLineValue << " | " << CorrectInstanceValue << endl;
    }

    EqualizePotGrid(PotGrid,Grid,X_Hor,Y_Ver); // Makes all Potential grids equal. Gives them a starting position
    //UpsideDownGrid(PotGrid,X_Hor,Y_Ver); // Turns the grid values upside down... because i have a feeling the issue is me doing this "Y_Ver-1" and other places just using the Y_Ver value.

    cout << "---" << endl;
    TempVal = CostValue(Grid,Y_Cost,Y_Cost_Instance,X_Hor,Y_Ver); // Initialize the grid and its value
    cout << "Starting positions costFunction currently is: " << TempVal << endl;
    for(int i=0; i<10; i++)
    {
        IzmVert[i] = TempVal;
    }

    // Obtain the amount of Black block clusters and store it in a massive. - We need a way to select a specific part of each column.
    int GridBlocks[50]; // Contains the information on how many Black square blocks are in a specific column.
    buildBlockSelection(DataX,X_Hor,X_Ver,GridBlocks);

    // GridBlocks - Black block cluster amount in each column | PotGrid - Grids that we are going to manipulate till we find the most optimal outcome. And hopefully the correct outcome to the game.
    // IzmVert - costFunction for each of the PotGrid options so far. // Izmaksu vertibas
    // X_Hor - Horizontal grid size | Y_Ver - Vertical grid size
    Optimizer_TheGameGrid(GridBlocks,PotGrid,IzmVert,X_Hor,Y_Ver,Y_Cost,Y_Cost_Instance);

    cout << "Solution found!" << endl;
    cout << "____" << endl;
    for(int i = 0; i < Y_Ver; i++) // Inverted so that the image starts from the bottom. Its easier for me to grasp that way
    {
        for(int j = 0; j < X_Hor; j++)
        {

            if(PotGrid[j][i][0] == 1)
            {
                SetConsoleTextAttribute(hConsole, 14);
                cout << PotGrid[j][i][0];
            } else {
                SetConsoleTextAttribute(hConsole, 1);
                cout << PotGrid[j][i][0];
            }
        }
        cout << endl;
    }
    SetConsoleTextAttribute(hConsole, 7); // Returns the color to default
    cout << "____" << endl;

    // IMPLEMENT THE LAHC optimizer for this...

    // For evaluation functions we obtain the sum value for each rows and columns. This value will be compared to the amount of Black Squares in the said row/column.
    // The module of the value is the correctness.
    // Any moves of the objects need to always be done with an allowed move!

    // Tab Search? LAHC (Late Acceptance Hill-Climbing) -> At least with this one i know how to use the F function.
    // Chose a random row that is valid
    // Chose an instance of Black Squares -> Evaluate the valid range where to relocate them to -> Randomly select a new position for them.
        // (Maybe not so randomly? What does the value F say?). There has to be a limit to how good or bad the new position can be. (How bad that is...)
    // Recalculate the F value for the rows that the Black Squares where relocated to AND from where they where taken from.
    // Chose a new row till the F == 0.

    // HEY TEST! V 1.2

    //cout << "Hello world! - This is a test!" << endl;
    return 0;
}

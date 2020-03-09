// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWorldList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();    

    Isograms = GetValidWords(HiddenWordBank);

    SetupGame();

    //PrintLine(TEXT("The number of possible words is %i"), HiddenWordBank.Num());
    //PrintLine(TEXT("The number of valid words is : %i"), GetValidWords(HiddenWordBank).Num());

}
 
void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ClearScreen();
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame()
{
    
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len();                 
    bGameOver = false;
 
    PrintLine(TEXT("Hi there! ")); //Welcome player to the game
    PrintLine(FString::Printf(TEXT("Guess the %i letter word!!!"), HiddenWord.Len())); //State the length of the current word to be guessed
    PrintLine(FString::Printf(TEXT("You currently have %i lives"), Lives)); //State the starting number of lives 
    
    //This line teaches how to format strings. Which is inserting data values inside strings to be printed to the terminal
    //For printing out strings (%s), it is required to 'dereference' the argument variable using a asterisk (*) operator (E.g. as below: *HiddenWord). This only applies to printing out strings
    PrintLine(FString::Printf(TEXT("The hidden word is: %s\nIt is %i characters long."), *HiddenWord, HiddenWord.Len()));//Debug Line to test whether the hidden word is selected correctly
    
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press Enter to play again"));   
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{

    if(!IsIsogram(Guess))
    {
        PrintLine(TEXT("There must be no repeating letters. Try again"));
        return; //Return statement here means "skip to end the of function without executing the remaining code after this if statement"
    }

    if(Guess.Len() != HiddenWord.Len())
    {
        PrintLine(FString::Printf(TEXT("The hidden word is %i letters long.\n"), HiddenWord.Len()));
        --Lives; //Decrement Lives by 1;
        return; //Return statement here means "skip to end the of function without executing the remaining code after this if statement"
    }


    FBullCowCount RealBullCows = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls & %i Cows"), RealBullCows.Bulls, RealBullCows.Cows);


    if(Guess == HiddenWord)//If word is an isogram and matches the word
    {
        PrintLine(TEXT("You win!!!\n"));
        EndGame();
        return;
    }
    
    if(Lives <= 0)//If remaining Lives is less or equal to 0
    {
        PrintLine(TEXT("You have ran out of lives!\n"));
        EndGame();
        return;
    }

    
    if(Lives > 0) //If player still has Lives
    {
        PrintLine(FString::Printf(TEXT("Lives left: %i\n"), Lives)); //Display current number of lives 
        PrintLine(TEXT("Try again\n"));
        return;
    }

    //Check if player has enough lives
    //If still have enough lives, let the player try to guess again
    //If not enough lives, display a Fail Message
    //If display fail message, ask player if they want to continue or not
    //If want to continue, run BeginPlay() again
    //If don't want to continue, quit the game
}

//Checks if the player input word is an isogram or not. An isogram means a word does not have repeating letters
bool UBullCowCartridge::IsIsogram(const FString& Guess) const
{

    for(int32 i = 0; i < Guess.Len() - 1; ++i)
    {
        for(int32 j = i; j < Guess.Len() - 1; ++j)
        {
            if(Guess[i] == Guess[j + 1])
            {
                return false;
            }
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& hiddenWordBankList) const
{
    TArray<FString> ValidWords;  

    //This is known as a range-based for loop. No need to 
    //think about indices since it iterates through every element of the loop
    for(FString wordFromBank : hiddenWordBankList)
    {
        if(wordFromBank.Len() >=  4 && wordFromBank.Len() <= 8 
            && IsIsogram(wordFromBank))
        { 
            ValidWords.Emplace(wordFromBank);
        }
    }

    return ValidWords;  
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{

    FBullCowCount Count; //Default initialized to 0

    for(int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        //This if statement checks the same index between the 
        if(Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
        }
        else
        {
            for(int32 GuessIdx = 0; GuessIdx < Guess.Len(); GuessIdx++)
            {
                if(Guess[GuessIndex] == HiddenWord[GuessIdx])
                {
                    Count.Cows++;
                    break; //Exit out of the for loop once a Cow has been found, so the loop does not have to waste time comparing letters 
                }
            }
        }
    }

    return Count;
}
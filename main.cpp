#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
int status = 0;

//main csv file to change
const string INPUTFILE = "games.csv";

// Structure to store user statistics
struct UserStats {
  int wins = 0;
  int losses = 0;
};

struct GameResult {
  int white_win = 0;
  int draw = 0;
  int black_win = 0;
  int moves_count = 0;

  // Default constructor
  GameResult() = default;

  // Constructor that accepts three integers
  GameResult(int white, int d, int black, int move)
      : white_win(white), draw(d), black_win(black), moves_count(move) {}
};

//clear screen
void clearScreen() {
#ifdef _WIN32
  (void)system("cls");
#else
  (void)system("clear");
#endif
}

// User Inquiries and Actions
int userInquiries();
void user_print_details(string id_name);
void findAndPrintTop3Users(const string &inputFile);
void printTop3Users(const vector<pair<string, UserStats>> &top3Users);
vector<pair<string, UserStats>> findTop3Users(const string inputFile);
        // Add game to the Data Set (csv file)
vector<string> get_game_details();
void add_game_to_csv(const string &filename, const vector<string> &gameData);

// Play 
       // Play a Two Player Game. 
vector<vector<string>> findFirstMoveAndCalculateProbabilities_printTopFiveMoves(const string &moveToSort);
void findGamesInVector_printTopFiveMoves(vector<vector<string>> &gamesData, const string moveToSort);
       // Play Versus Computer
void play_against_computer(string b_or_w);
vector<vector<string>> find_bestNextMove_and_Return_sortedVector(const string &inputFile, const string &moveToSort, string &bestNextMove, string b_or_w);
void findGamesInVector_againstComputer(vector<vector<string>> &gamesData, const string moveToSort, string &bestNextMove, string b_or_w);
vector<vector<string>> FirstIteration_ReturnSortedVector_findBestMove(string &bestNextMove);

// Opening Inquiries
void openingInquiries();
string getOpeningNameForMoves(const vector<vector<string>> &gamesData, const string &moves);
void countOpeningVariations(string name);

string generate_moves(const vector<vector<string>> &games, const string &opening_name);

//Sorting and Search Algorithms / Functions
void mergeSort(vector<vector<string>> &data, int left, int right);
void merge(vector<vector<string>> &data, int left, int mid, int right);
int findFirst(const vector<vector<string>> &gamesData, const string &moveToSort); //Binary search
int binarySearch_movesname(const vector<string> &moves, const vector<vector<string>> &gamesData, int left, int right, const string &target);
vector<vector<string>> findAnd_ShortenData(vector<vector<string>> &gamesData, const string moveToSort);
void mergeSort_for_variations(vector<pair<string, int>> &data, int left, int right);
void merge_for_variations(vector<pair<string, int>> &data, int left, int mid, int right);
vector<vector<string>> merge_movesname(const vector<vector<string>> &left, const vector<vector<string>> &right);
vector<vector<string>> mergeSort_movesname(const vector<vector<string>> &arr);
void mergeSort_for_top3(vector<pair<string, UserStats>> &arr, int l, int r);
void merge_for_top_3(vector<pair<string, UserStats>> &arr, int l, int m, int r);

//String manipulation and other functions
string getFirstWord(const string &str);
vector<string> split(const string &str, char delimiter);
string extractNextMove(const string &moveToSort, const string &currentMove);
void updateResult(unordered_map<string, GameResult> &results, const string &key, int white_win, int draw, int black_win);


int userInquiries() {
  int userChoice;
  cout << "User Inquiries:\n";
  cout << "1. Enter user name for analysis\n";
  cout << "2. Top 3 users\n";
  cout << "3. To add a new game to the dataset\n";
  cin >> userChoice;

  switch (userChoice) {
  case 1: {
    cin.ignore();
    string id_name;
    cout << "Enter user ID : ";
    getline(cin, id_name);
    user_print_details(id_name);
    // Perform analysis on the specified user
    // Implement the functionality for user analysis
    break;
  }
  case 2: {
    // Find and output top 3 users based on win-loss record and number of games
    cin.ignore();
    findAndPrintTop3Users(INPUTFILE);
    cout << endl;

    break;
  }
  case 3: {
    cin.ignore();
    // Get game details from user input
    vector<string> gameData = get_game_details();

    // Add the new game to the CSV file
    add_game_to_csv(INPUTFILE, gameData);
  }
  default:
    cout << "Invalid choice.\n";
    break;
  }
  return 0;
}

void openingInquiries() {
  int openingChoice;
  cout << "Opening Inquiries:\n";
  cout << "1. Enter opening name and get other sub variations names with the "
          "number of games.\n";
  cout << "2. Enter opening name and get opening moves.\n";
  cout << "3. Enter game string and get the opening name\n";
  cin >> openingChoice;

  switch (openingChoice) {
  case 1: {
    cin.ignore();
    string userInputOpening;
    cout << "Enter opening name: ";
    getline(cin, userInputOpening);
    // Call the function with user input
    countOpeningVariations(userInputOpening);
    break;
  }

  case 2: {
    cin.ignore(); // Ignore any residual newline characters in the input buffer
    string opening_name;
    cout << "Enter opening name: ";
    getline(cin, opening_name);

    vector<vector<string>> gamesData;

    ifstream file(INPUTFILE);
    if (file.is_open()) {
      string line;
      getline(file, line);
      while (getline(file, line)) {
        gamesData.push_back(split(line, ','));
      }
      file.close();
      generate_moves(gamesData, opening_name);
    } else {
      cerr << "Unable to open file: " << INPUTFILE << endl;
    }
    break;
  }

  case 3: {
    cin.ignore();
    vector<vector<string>> gamesData;
    ifstream file(INPUTFILE);

    if (!file.is_open()) {
      cerr << "Unable to open file." << endl;
      return;
    }
    string line;
    while (getline(file, line)) {
      gamesData.push_back(split(line, ','));
    }
    file.close();

    // Get moves from the user
    string moves;
    cout << "Enter moves separated by space: ";
    getline(cin, moves);

    // Get the most common opening name for the provided moves
    string mostCommonOpening = getOpeningNameForMoves(gamesData, moves); 
    cout << "Most common opening for the provided moves: " << mostCommonOpening
         << endl;
    break;
  }
  default:
    cout << "Invalid choice.\n";
    break;
  }
}

void playGame() {
  int playChoice;
  cout << "Play Game:\n";
  cout << "Here after each move win, loss and draw Probabilites will be "
          "printed\n";
  cout << "1. Play a 2 player game. \n";
  cout << "2. Play as black or white against computer.\n";
  cin >> playChoice;

  switch (playChoice) {
  case 1: {
    // 1. Play a 2 player game.
    cin.ignore();
    string moveToSort;
    vector<vector<string>> gamesData =   findFirstMoveAndCalculateProbabilities_printTopFiveMoves(moveToSort);
    // if the user is asking for the best move to play from white, then you need
    // to traverse through the csv, and paste it in a vector, while doing so you
    // just need to make keys, and then those keys will make sure that it
    // gives the best moveToSort
cout<< "Enter the move number 1 : ";
cin>> moveToSort;
findGamesInVector_printTopFiveMoves(gamesData, moveToSort);

    int i = 2;
    while (1) {
      string temp;
      cout << "\n**Enter -1 to exit the game**\n";
      cout << "\nEnter the move number " << i << ": ";
      cin >> temp;
      moveToSort = moveToSort + " " + temp;
      cout << moveToSort << endl;

      if(moveToSort == "-1"){
        return;
      }

    findGamesInVector_printTopFiveMoves(gamesData, moveToSort);
      if (status == -1) {
        return;
      }
      i++;
    }
    break;
  }
  case 2: {
    cin.ignore();
    string b_or_w;
    cout << "Play as White or Black? [b/w]";
    getline(cin, b_or_w);
    play_against_computer(b_or_w);
    break;
  }
  default:
    cout << "Invalid choice.\n";
    break;
  }
}

void play_against_computer(string b_or_w) {
// Assuming you have the file path defined here
  string moveToSort, bestNextMove;
  vector<vector<string>> gamesData;
  int i = 2;

  if (b_or_w == "b") {
    //The user plays as black, computer as white.
    gamesData = FirstIteration_ReturnSortedVector_findBestMove(bestNextMove);
    // Now gamesData is sorted with the move which the computer has chosen
    cout << "The computer played: " << bestNextMove << endl;
    string enitre_string = bestNextMove;
    while (true) {
      // Computer's move 
      cout << "\n**Enter -1 to exit the game**\n";
      cout << "\nEnter move number : " << i << endl;
      cin >> moveToSort;

      if(moveToSort == "-1"){
        return;
      }
      enitre_string = enitre_string + " " + moveToSort;

      // Find best move for user (white)
      findGamesInVector_againstComputer(gamesData, enitre_string, bestNextMove, "w");
      if (status != -1) {
        cout << "\nThe computer played: " << bestNextMove << endl;
        enitre_string = enitre_string + " " + bestNextMove;
        cout << "Move order: " << enitre_string << endl;
      } else {
        return;
      }
      if (status == -1) {
        return;
      }
      i += 2;
    }
  } else if (b_or_w == "w") {
    // User plays white, computer plays black
    cout << "Enter the move number 1: ";
    cin >> moveToSort;
    gamesData = find_bestNextMove_and_Return_sortedVector(INPUTFILE, moveToSort, bestNextMove, "b");
    string enitre_string = moveToSort + " " + bestNextMove;

    while (true) {

      cout << "\nThe computer played: " << bestNextMove << endl;
      i += 2;
      cout << "Moves played: " << enitre_string << endl;

      cout << "\n**Enter -1 to exit the game**\n";
      cout << "\nEnter move number " << i - 1 << ": ";
      cin >> moveToSort;

      if(moveToSort == "-1"){
        return;
      }

      // User's move
      enitre_string = enitre_string + " " + moveToSort;
      cout << "Moves played: " << enitre_string << endl;
      // Find best move for computer (black)
      findGamesInVector_againstComputer(gamesData, enitre_string, bestNextMove, "b");
      if (status == -1) {
        return;
      }
      enitre_string = enitre_string + " " + bestNextMove;
    }
  }
}

//Main Function
int main()
{
  while (true) {
    cout << endl;
    int choice;
    cout << "Main Menu:\n";
    cout << "1. User Inquiries & Actions\n";
    cout << "2. Opening Inquiries\n";
    cout << "3. Play Game\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Invalid input. Please enter a number.\n";
      continue; // Restart the loop to prompt for input again
    }

    switch (choice) {
    case 1:
      clearScreen();
      userInquiries();
      break;
    case 2:
      clearScreen();
      openingInquiries();
      break;
    case 3:
      clearScreen();
      playGame();
      break;
    case 4:
      cout << "Exiting...\n";
      return 0;
    default:
      cout << "Invalid choice. Please enter a number between 1 and 4.\n";
      break;
    }
  }

  return 0;
}

string generate_moves(const vector<vector<string>> &games, const string &opening_name) {
  string moves;

  for (const auto &game : games) {
    if (game.size() >= 16 && game[14] == opening_name) {
      cout << "Found matching opening: " << opening_name << endl;

      // Extracting moves before the first ply
      vector<string> movesData = split(game[12], ' ');
      moves = movesData[0]; // Take the first move
      for (int i = 1; i < stoi(game[15]); ++i) {
        moves += " " + movesData[i]; // Concatenate moves up to ply
      }

      cout << "Moves: " << moves << endl;
      return moves;
    }
  }

  cout << "No game found for the given opening name." << endl;
  return "";
}

void user_print_details(string id) {

  vector<vector<string>> gamesData;
  ifstream file(INPUTFILE);
  if (file.is_open()) {
    string line;
    getline(file, line);
    while (getline(file, line)) {
      gamesData.push_back(split(line, ','));
    }
    file.close();
  } else {
    cerr << "Unable to open file: " << INPUTFILE << endl;
    return;
  }

  float total_games = 0;
  float wins_as_white = 0;
  float wins_as_black = 0;
  float losses = 0;
  float draws = 0;
  float rating = 0;

  for (const auto &game : gamesData) {
    if (game[8] == id || game[10] == id) {
      total_games++;
      if (game[8] == id) {
        if (game[6] == "white")
          wins_as_white++;
        else if (game[6] == "black")
          losses++;
        else if (game[6] == "draw")
          draws++;
      } else if (game[10] == id) {
        if (game[6] == "black")
          wins_as_black++;
        else if (game[6] == "white")
          losses++;
        else if (game[6] == "draw")
          draws++;
      }

      cout << "Game data for user ID: " << id << endl;
      for (const auto &data : game) {
        cout << data << " ";
      }
      cout << endl;
      cout << endl;
    }

    if (game[8] == id)
      rating = stoi(game[9]) + rating;
    if (game[10] == id)
      rating = stoi(game[11]) + rating;
  }

  float white_win_chance = (wins_as_white / total_games) * 100.00;
  float draw_chance = (draws / total_games) * 100.00;
  float black_win_chance = (wins_as_black / total_games) * 100.00;

  if (total_games == 0) {
    cout << "User Id not found in Database" << endl;
    return;
  }

  cout << "Total number of games played: " << total_games << endl;
  cout << "Number of losses: " << losses << endl;
  cout << "Number of draws: " << draws << endl;
  cout << "Number of wins: " << wins_as_white + wins_as_black << endl;
  cout << "Wins as white: " << wins_as_white << endl;
  cout << "Wins as black: " << wins_as_black << endl;
  cout << "Win Probability - White: " << white_win_chance
       << "% | Draw: " << draw_chance << "% | Black: " << black_win_chance
       << "%" << endl;
  cout << "Average Rating: " << rating / total_games << endl;
}

// Function to find and print top 3 users from CSV file
void findAndPrintTop3Users(const string &INPUTFILE) {
  vector<pair<string, UserStats>> topUsers = findTop3Users(INPUTFILE);
  printTop3Users(topUsers);
}

vector<pair<string, UserStats>> findTop3Users(const string INPUTFILE) {
  unordered_map<string, UserStats> userStats;

  ifstream file(INPUTFILE);
  if (!file.is_open()) {
    cerr << "Unable to open file: " << INPUTFILE << endl;
    return {};
  }

  string line;
  getline(file, line);
  while (getline(file, line)) {
    vector<string> rowData = split(line, ',');

    string whitePlayer = rowData[8];
    string blackPlayer = rowData[10];

    if (rowData[6] == "white") {
      userStats[whitePlayer].wins++;
      userStats[blackPlayer].losses++;
    } else if (rowData[6] == "black") {
      userStats[blackPlayer].wins++;
      userStats[whitePlayer].losses++;
    }
  }
  file.close();

  // Filter users with more wins than losses
  vector<pair<string, UserStats>> filteredUsers;
  for (const auto &entry : userStats) {
    const string &user = entry.first;
    const UserStats &stats = entry.second;
    if (stats.wins > stats.losses) {
      filteredUsers.push_back({user, stats});
    }
  }

  // Perform merge sort on filtered users based on the number of wins
  mergeSort_for_top3(filteredUsers, 0, filteredUsers.size() - 1);

  return filteredUsers;
}

//merge sort modified for specific needs.
void merge_for_variations(vector<pair<string, int>> &data, int left, int mid,
                          int right) {
  int n1 = mid - left + 1;
  int n2 = right - mid;

  vector<pair<string, int>> L(n1), R(n2);
  for (int i = 0; i < n1; ++i)
    L[i] = data[left + i];
  for (int j = 0; j < n2; ++j)
    R[j] = data[mid + 1 + j];

  int i = 0, j = 0, k = left;
  while (i < n1 && j < n2) {
    if (L[i].second >= R[j].second) {
      data[k] = L[i];
      ++i;
    } else {
      data[k] = R[j];
      ++j;
    }
    ++k;
  }

  while (i < n1) {
    data[k] = L[i];
    ++i;
    ++k;
  }
  while (j < n2) {
    data[k] = R[j];
    ++j;
    ++k;
  }
}

void updateResult(unordered_map<string, GameResult> &results, const string &key, int white_win, int draw, int black_win) {
  // If the key exists, update the result
  if (results.find(key) != results.end()) {
    results[key].white_win += white_win;
    results[key].draw += draw;
    results[key].black_win += black_win;
    results[key].moves_count++;
  } else {
    // If the key does not exist, insert a new result
    results[key] = GameResult(white_win, draw, black_win, 1);
  }
}

vector<string> split(const string &str, char delimiter) {
  vector<string> tokens;
  stringstream ss(str);
  string token;
  while (getline(ss, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}



// Function to merge two sorted vectors
vector<vector<string>> merge_movesname(const vector<vector<string>> &left, const vector<vector<string>> &right)
{
  vector<vector<string>> result;
  size_t leftIndex = 0, rightIndex = 0;
  while (leftIndex < left.size() && rightIndex < right.size()) {
    if (left[leftIndex][12] < right[rightIndex][12]) {
      result.push_back(left[leftIndex]);
      leftIndex++;
    } else {
      result.push_back(right[rightIndex]);
      rightIndex++;
    }
  }
  while (leftIndex < left.size()) {
    result.push_back(left[leftIndex]);
    leftIndex++;
  }
  while (rightIndex < right.size()) {
    result.push_back(right[rightIndex]);
    rightIndex++;
  }
  return result;
}

// Function to perform merge sort
vector<vector<string>> mergeSort_movesname(const vector<vector<string>> &arr) 
{
  if (arr.size() <= 1) {
    return arr;
  }
  size_t mid = arr.size() / 2;
  vector<vector<string>> left(arr.begin(), arr.begin() + mid);
  vector<vector<string>> right(arr.begin() + mid, arr.end());
  left = mergeSort_movesname(left);
  right = mergeSort_movesname(right);
  return merge_movesname(left, right);
}

// Function to perform binary search for finding matching moves
int binarySearch_movesname(const vector<string> &moves, const vector<vector<string>> &gamesData, int left, int right, const string &target)
{
  while (left <= right) {
    int mid = left + (right - left) / 2;
    if (gamesData[mid][12] == target) {
      return mid;
    } else if (gamesData[mid][12] < target) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }
  return -1;
}

// Function to get the opening name for a given set of moves
string getOpeningNameForMoves(const vector<vector<string>> &gamesData, const string &moves) 
{
  vector<string> moveList = split(moves, ' ');

  // Sort the game data based on moves using merge sort
  vector<vector<string>> sortedGamesData = mergeSort_movesname(gamesData);

  // Find the index of the first occurrence of moves in the game data
  int firstIndex = binarySearch_movesname(moveList, sortedGamesData, 0, sortedGamesData.size() - 1, moves);
  if (firstIndex == -1) {
    return "No matching opening found.";
  }

  // Count occurrences of opening names for matching moves
  unordered_map<string, int> openingCounts;
  int index = firstIndex;
  while (index < gamesData.size() && sortedGamesData[index][12] == moves) {
    openingCounts[sortedGamesData[index][14]]++;

    index++;
  }

  // Choose the opening name with the highest count
  string mostCommonOpening;
  int maxCount = 0;
  for (const auto &entry : openingCounts) {
    if (entry.second > maxCount) {
      maxCount = entry.second;
      mostCommonOpening = entry.first;
    }
  }

  return mostCommonOpening;
}

// Extract the next move as a string and return it
string extractNextMove(const string &moveToSort, const string &currentMove) 
{
  // Find the length of the current move
  int currentMoveLength = currentMove.length();

  // Initialize variables to track the positions
  int currentPosition = 0;
  int nextSpacePosition = -1;

  // Find the position of the current move
  while (currentPosition < moveToSort.length()) {
    bool moveMatched = true;
    // Check if the substring starting at currentPosition matches the current
    // move
    for (int i = 0; i < currentMoveLength; ++i) {
      if (currentPosition + i >= moveToSort.length() ||
          moveToSort[currentPosition + i] != currentMove[i]) {
        moveMatched = false;
        break;
      }
    }

    // If the move is matched, find the position of the next space after it
    if (moveMatched) {
      nextSpacePosition = -1;
      for (int i = currentPosition + currentMoveLength; i < moveToSort.length();
           ++i) {
        if (moveToSort[i] == ' ') {
          nextSpacePosition = i;
          break;
        }
      }
      break; // Break out of the loop since we found the move
    }
    currentPosition++;
  }

  // If the current move was not found or there's no space after it, return an
  // empty string
  if (currentPosition == moveToSort.length() || nextSpacePosition == -1) {
    return "";
  }

  // Extract the substring between the current move and the next space
  return moveToSort.substr(currentPosition + currentMoveLength,
                           nextSpacePosition - currentPosition -
                               currentMoveLength);
}

// Merge function for merge sort
void merge(vector<vector<string>> &data, int left, int mid, int right)
{
  int n1 = mid - left + 1;
  int n2 = right - mid;

  vector<vector<string>> L(n1), R(n2);
  for (int i = 0; i < n1; ++i)
    L[i] = data[left + i];
  for (int j = 0; j < n2; ++j)
    R[j] = data[mid + 1 + j];

  int i = 0, j = 0, k = left;
  while (i < n1 && j < n2) {
    if (L[i][12] <= R[j][12]) {
      data[k] = L[i];
      ++i;
    } else {
      data[k] = R[j];
      ++j;
    }
    ++k;
  }

  while (i < n1) {
    data[k] = L[i];
    ++i;
    ++k;
  }
  while (j < n2) {
    data[k] = R[j];
    ++j;
    ++k;
  }
}


// Merge sort function
void mergeSort(vector<vector<string>> &data, int left, int right)
{
  if (left < right) {
    int mid = left + (right - left) / 2;
    mergeSort(data, left, mid);
    mergeSort(data, mid + 1, right);

    merge(data, left, mid, right);
  }
}

/*using binary search it finds the first index where a specific move is
starting. This is made possible because in the first iteration we are sorting
the vector*/
int findFirst(const vector<vector<string>> &gamesData, const string &moveToSort)
{
  int low = 0, high = gamesData.size() - 1;
  while (low <= high) {
    int mid = low + (high - low) / 2;
    if (gamesData[mid][12].compare(0, moveToSort.size(), moveToSort) < 0) {
      low = mid + 1;
    } else if (gamesData[mid][12].compare(0, moveToSort.size(), moveToSort) >
               0) {
      high = mid - 1;
    } else if (mid == 0 || gamesData[mid - 1][12].compare(0, moveToSort.size(),
                                                          moveToSort) != 0) {
      return mid;
    } else {
      high = mid - 1;
    }
  }
  return -1;
}

vector<vector<string>> findFirstMoveAndCalculateProbabilities_printTopFiveMoves(const string &moveToSort)
{
  unordered_map<string, GameResult> results;
  float white_win_for_current_move = 0;
  float draw_count_for_current_move = 0;
  float black_win_for_current_move = 0;

  ifstream file(INPUTFILE);
  vector<vector<string>> data;
  string line;

  while (getline(file, line)) {
    vector<string> rowData = split(line, ',');
    // Check if the first move of the game matches the specified move
    string nextMove = getFirstWord(rowData[12]);

    //if (rowData[5] != "outoftime") {
      if (rowData[6] == "white") {
        updateResult(results, nextMove, 1, 0, 0);
        white_win_for_current_move ++;
      } else if (rowData[6] == "black") {
        updateResult(results, nextMove, 0, 0, 1);
        black_win_for_current_move ++;
      } else {
        updateResult(results, nextMove, 0, 1, 0);
        draw_count_for_current_move ++;
      }
    //}
    data.push_back(rowData);
  }

  file.close();

  float total_games = white_win_for_current_move + draw_count_for_current_move + black_win_for_current_move;
  cout << "\nTotal games in the data set: " << total_games << endl;
  if (total_games == 0) {
    cout << "No games found for the specified move(s)." << endl;
    return data;
  }

  // Output the results and probabilities
  cout << "\nTotal games in the data set for the moves: " << total_games << endl;
  cout << "Current white win probability: " << (white_win_for_current_move / total_games) * 100.00 << "%\n";
  cout << "Current draw probability: "  << (draw_count_for_current_move / total_games) * 100.00 << "%\n";
  cout << "Current black win probability: " << (black_win_for_current_move / total_games) * 100.00 << "%\n";

  pair<string, GameResult> topMovesArray[5];

    // Iterate over the results to find the top 5 moves
    for (const auto &pair : results) {
        for (int i = 0; i < 5; ++i) {
          if (pair.second.moves_count > topMovesArray[i].second.moves_count) {
            for (int j = 4; j > i; --j) {
              topMovesArray[j] = topMovesArray[j - 1];
            }
            topMovesArray[i] = pair;
            break;
          }
        }
    }


  cout << "\nTop 5 moves played:" << endl;
  for (int i = 0; i < 5; ++i) {
    if (topMovesArray[i].first == "") {
      break;
    } else {
      cout << "Move: " << topMovesArray[i].first <<  " count: " << topMovesArray[i].second.moves_count << endl;
    }
  }

  mergeSort(data, 0, data.size() - 1);

  return data;
}

void findGamesInVector_printTopFiveMoves(vector<vector<string>> &gamesData, const string moveToSort)
{
  unordered_map<string, GameResult> results;
  vector<vector<string>> sortedGames;

  // Use binary search to find the range of relevant games
  int firstIndex = findFirst(gamesData, moveToSort);
  if (firstIndex == -1) {
    cout << "No games found for the specified move(s)." << endl;
    return;
  }

  float white_win_for_current_move = 0;
  float draw_count_for_current_move = 0;
  float black_win_for_current_move = 0;

  // Now iterate only through the range of relevant games
  for (int i = firstIndex; i < gamesData.size(); ++i) {
    const auto &rowData = gamesData[i];

    if (rowData[12].substr(0, moveToSort.size()) == moveToSort) {
      //if (rowData[5] != "outoftime") {
        if (rowData[6] == "white") {
          updateResult( results, extractNextMove(rowData[12],rowData[12].substr(0, moveToSort.size()) + ' '), 1, 0, 0);
          white_win_for_current_move++;
        } else if (rowData[6] == "black") {
          updateResult( results, extractNextMove(rowData[12], rowData[12].substr(0, moveToSort.size()) + ' '), 0, 0, 1);
          black_win_for_current_move++;
        } else {
          updateResult(results, extractNextMove(rowData[12], rowData[12].substr(0, moveToSort.size()) + ' '), 0, 1, 0);
          draw_count_for_current_move++;
        }
      //}
      sortedGames.push_back(rowData);
    }
  }

pair<string, GameResult> topMovesArray[5];

    // Iterate over the results to find the top 5 moves
    for (const auto &pair : results) {
        for (int i = 0; i < 5; ++i) {
          if (pair.second.moves_count > topMovesArray[i].second.moves_count) {
            for (int j = 4; j > i; --j) {
              topMovesArray[j] = topMovesArray[j - 1];
            }
            topMovesArray[i] = pair;
            break;
          }
        }
    }

  // Output the results and probabilities
  float total_games = white_win_for_current_move + draw_count_for_current_move +  black_win_for_current_move;
  cout << "\nTotal games in the data set for the moves: " << total_games
       << endl;
  cout << "Current white win probability: "
       << (white_win_for_current_move / total_games) * 100.00 << "%\n";
  cout << "Current draw probability: "
       << (draw_count_for_current_move / total_games) * 100.00 << "%\n";
  cout << "Current black win probability: "
       << (black_win_for_current_move / total_games) * 100.00 << "%\n";

//Print the top 5 moves played
  cout << "\nTop 5 moves played:" << endl;
  for (int i = 0; i < 5; ++i) {
    if (topMovesArray[i].first == "") {
      break;
    } else {
      cout << "Move: " << topMovesArray[i].first <<  " count: " << topMovesArray[i].second.moves_count << endl;
    }
  }
  gamesData = sortedGames;
}

void findGamesInVector_againstComputer(vector<vector<string>> &gamesData, const string moveToSort, string &bestNextMove, string b_or_w)
{
  status = 0;
  unordered_map<string, GameResult> results;
  vector<vector<string>> sortedGames;

  int firstIndex = findFirst(gamesData, moveToSort);
  if (firstIndex == -1) {
    status = -1;
    cout << "No games found for the specified move(s)." << endl;
    return;
  }

  float white_win_for_current_move = 0;
  float draw_count_for_current_move = 0;
  float black_win_for_current_move = 0;

  // Now iterate only through the range of relevant games
  for (int i = firstIndex; i < gamesData.size(); ++i) {
    const auto &rowData = gamesData[i];

    if (rowData[12].substr(0, moveToSort.size()) == moveToSort) {
      //if (rowData[5] != "outoftime"){
        //it will extract the next move, and store in the map. then it will find the best move to be played in the map.
        string nextMove = extractNextMove(rowData[12], rowData[12].substr(0, moveToSort.size()) + ' ');
        if (rowData[6] == "white") {
          updateResult(results, nextMove, 1, 0, 0);
          white_win_for_current_move++;
        } else if (rowData[6] == "black") {
          updateResult(results, nextMove, 0, 0, 1);
          black_win_for_current_move++;
        } else {
          updateResult(results, nextMove, 0, 1, 0);
          draw_count_for_current_move++;
        }
      //}
      sortedGames.push_back(rowData);
    }
  }

  // find the top move for black
  if (b_or_w == "b") {
    int maxBlackWins = -1;

    for (const auto &pair : results) {
      if (pair.second.white_win < (pair.second.black_win + pair.second.draw)) {
        if (pair.second.black_win > maxBlackWins) {
          maxBlackWins = pair.second.black_win;
          bestNextMove = pair.first;
        }
      }
    }
    //In case no games are better for White
    if(maxBlackWins == -1){
      for(const auto &pair : results){
        if(pair.second.moves_count > maxBlackWins){
          maxBlackWins = pair.second.moves_count;
          bestNextMove = pair.first;
        }
      }
    }

    if(maxBlackWins == -1){
      cout<<"The data set is exhausted, the computer has no moves to play.";
      return;
    }   
//find best move for white.
  } else {
    int maxWhiteWins = -1;

    for (const auto &pair : results) {
      if (pair.second.black_win < (pair.second.white_win + pair.second.draw)) {
        if (pair.second.white_win > maxWhiteWins) {
          maxWhiteWins = pair.second.white_win;
          bestNextMove = pair.first;
        }
      }
    }
    //In case no games are better for White
    if(maxWhiteWins == -1){
      for(const auto &pair : results){
        if(pair.second.moves_count > maxWhiteWins){
          maxWhiteWins = pair.second.moves_count;
          bestNextMove = pair.first;
        }
      }
    }

    if(maxWhiteWins == -1){
      cout<<"The data set is exhausted, the computer has no moves to play.";
      return; 
    }

  }

  float total_games = white_win_for_current_move + draw_count_for_current_move + black_win_for_current_move;
  cout << "\nTotal games in the data set for the moves: " << total_games << endl;
  if (total_games == 0) {
    cout << "No games found for the specified move(s)." << endl;
    return;
  }

  cout << "Current white win probability: " << (white_win_for_current_move / total_games) * 100.00 << "%\n";
  cout << "Current draw probability: "  << (draw_count_for_current_move / total_games) * 100.00 << "%\n";
  cout << "Current black win probability: " << (black_win_for_current_move / total_games) * 100.00 << "%\n";

  gamesData = sortedGames;
}


// it refers to a CSV file, and creates a vector of vector of strings and sorts it.
/*it also refers to the next moveToSort and adds them in a hashtable, whil
incrementing the number of white wins, black wins and draws for that particular
move*/
vector<vector<string>> find_bestNextMove_and_Return_sortedVector(const string &INPUTFILE, const string &moveToSort, string &bestNextMove, string b_or_w)
{
  unordered_map<string, GameResult> results;

  float white_win_for_current_move = 0;
  float draw_count_for_current_move = 0;
  float black_win_for_current_move = 0;

  ifstream file(INPUTFILE);
  vector<vector<string>> data;
  string line;
  while (getline(file, line)) {
    vector<string> rowData = split(line, ',');
    // Check if the first move of the game matches the specified move
    if (rowData[12].substr(0, moveToSort.size()) == moveToSort) {
      //if (rowData[5] != "outoftime") {
        string nextMove = extractNextMove( rowData[12], rowData[12].substr(0, moveToSort.size()) + ' ');
        if (rowData[6] == "white") {
          updateResult(results, nextMove, 1, 0, 0);
          white_win_for_current_move++;
        } else if (rowData[6] == "black") {
          updateResult(results, nextMove, 0, 0, 1);
          black_win_for_current_move++;
        } else {
          updateResult(results, nextMove, 0, 1, 0);
          draw_count_for_current_move++;
        }
      //}
      // Add the entire row to data
      data.push_back(rowData);
    }
  }
  file.close();

  if (b_or_w == "b") {
    // find the top move
    int maxBlackWins = -1;

    for (const auto &pair : results) {
      if (pair.second.white_win < (pair.second.black_win + pair.second.draw)) {
        // Check if this move has more black wins than the current max
        if (pair.second.black_win > maxBlackWins) {
          maxBlackWins = pair.second.black_win;
          bestNextMove = pair.first;
        }
      }
    }

    //In case no games are better for White
    if(maxBlackWins == -1){
      for(const auto &pair : results){
        if(pair.second.moves_count > maxBlackWins){
          maxBlackWins = pair.second.moves_count;
          bestNextMove = pair.first;
        }
      }
    }

    if(maxBlackWins == -1){
      cout<<"The data set is exhausted, the computer has no moves to play.";
      return data;
    }

  } else {
    int maxWhiteWins = -1;
    for (const auto &pair : results) {
      if (pair.second.black_win < (pair.second.white_win + pair.second.draw)) {
        if (pair.second.white_win > maxWhiteWins) {
          maxWhiteWins = pair.second.white_win;
          bestNextMove = pair.first;
        }
      }
    }
    //In case no games are better for White
    if(maxWhiteWins == -1){
      for(const auto &pair : results){
        if(pair.second.moves_count > maxWhiteWins){
          maxWhiteWins = pair.second.moves_count;
          bestNextMove = pair.first;
        }
      }
    }
    if(maxWhiteWins == -1){
      cout<<"The data set is exhausted, the computer has no moves to play.";
      return data; 
    }
  }

  float total_games = white_win_for_current_move + draw_count_for_current_move + black_win_for_current_move;
  cout << "\nTotal games in the data set for the moves: " << total_games << endl;
  if (total_games == 0) {
    cout << "No games found for the specified move(s)." << endl;
    return data;
  }

  cout << "Current white win probability: " << (white_win_for_current_move / total_games) * 100.00 << "%\n";
  cout << "Current draw probability: "  << (draw_count_for_current_move / total_games) * 100.00 << "%\n";
  cout << "Current black win probability: " << (black_win_for_current_move / total_games) * 100.00 << "%\n";

  // Sort the data using merge sort
  mergeSort(data, 0, data.size() - 1);

  // Write sorted data to a new CSV file if needed
  return data;
}

string getFirstWord(const string &str)
{
  // Find the position of the first space character
  size_t spacePos = str.find(' ');

  // If no space is found, return the entire string
  if (spacePos == string::npos) {
    return str;
  }

  // Extract the substring from the beginning of the string up to the space
  return str.substr(0, spacePos);
}

vector<vector<string>> FirstIteration_ReturnSortedVector_findBestMove( string &bestNextMove)
{ 
  unordered_map<string, GameResult> results;
  ifstream file(INPUTFILE);
  vector<vector<string>> data;
  string line;

  while (getline(file, line)){

    vector<string> rowData = split(line, ',');
    // Check if the first move of the game matches the specified move
    string nextMove = getFirstWord(rowData[12]);

   // if (rowData[5] != "outoftime") {
      if (rowData[6] == "white") {
        updateResult(results, nextMove, 1, 0, 0);
      } else if (rowData[6] == "black") {
        updateResult(results, nextMove, 0, 0, 1);
      } else {
        updateResult(results, nextMove, 0, 1, 0);
      }
    //}
    data.push_back(rowData);
  }

  file.close();

  int maxWhiteWins = -1;

  // find the best move, from all the moves
  for (const auto &pair : results) {
    if (pair.second.black_win < (pair.second.white_win + pair.second.draw)) {
      if (pair.second.white_win > maxWhiteWins) {
        maxWhiteWins = pair.second.white_win;
        bestNextMove = pair.first;
      }
    }
  }

  // Sort the vector
  mergeSort(data, 0, data.size() - 1);
  // perform a binary search, and copy paste the vector in the vector to return.
  return findAnd_ShortenData(data, bestNextMove);
}

vector<vector<string>> findAnd_ShortenData(vector<vector<string>> &gamesData, const string moveToSort)
{
  vector<vector<string>> sortedGames;

  int firstIndex = findFirst(gamesData, moveToSort);
  if (firstIndex == -1) {
    cout << "No games found for the specified move(s)." << endl;
    return sortedGames;
  }

  // Now iterate only through the range of relevant games
  for (int i = firstIndex; i < gamesData.size(); ++i) {

    const auto &rowData = gamesData[i];
    if (rowData[12].substr(0, moveToSort.size()) == moveToSort) {
      sortedGames.push_back(rowData);
    } else {
      break;
    }
  }
  return sortedGames;
}

// Merge function for merge sort
void merge_for_top_3(vector<pair<string, UserStats>> &arr, int l, int m, int r)
{
  int n1 = m - l + 1;
  int n2 = r - m;

  vector<pair<string, UserStats>> L(n1), R(n2);

  for (int i = 0; i < n1; i++)
    L[i] = arr[l + i];
  for (int j = 0; j < n2; j++)
    R[j] = arr[m + 1 + j];

  int i = 0;
  int j = 0;
  int k = l;

  while (i < n1 && j < n2) {
    if (L[i].second.wins >= R[j].second.wins) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }
}

// Merge sort function
void mergeSort_for_top3(vector<pair<string, UserStats>> &arr, int l, int r)
{
  if (l >= r)
    return;
  int m = l + (r - l) / 2;
  mergeSort_for_top3(arr, l, m);
  mergeSort_for_top3(arr, m + 1, r);
  merge_for_top_3(arr, l, m, r);
}


// Function to print top 3 users
void printTop3Users(const vector<pair<string, UserStats>> &top3Users)
{
  cout << "Top 3 Users with More Wins than Losses:\n";
  for (size_t i = 0; i < min(top3Users.size(), size_t(3)); ++i) {
    const string &user = top3Users[i].first;
    const UserStats &stats = top3Users[i].second;
    cout << "User: " << user << ", Wins: " << stats.wins
         << ", Losses: " << stats.losses << endl;
  }
}


// Merge sort function
void mergeSort_for_variations(vector<pair<string, int>> &data, int left, int right)
{
  if (left < right) {
    int mid = left + (right - left) / 2;

    mergeSort_for_variations(data, left, mid);
    mergeSort_for_variations(data, mid + 1, right);

    merge_for_variations(data, left, mid, right);
  }
}

void countOpeningVariations(string name)
{
  string userInputOpening;
  userInputOpening = name;

  unordered_map<string, int> openingCounts;

  ifstream file(INPUTFILE);
  string line;
  getline(file, line);
  while (getline(file, line)) {
    // Parse the line and extract the opening name from the "opening_name"
    // column
    vector<string> columns = split(line, ',');
    if (columns.size() >=
        15) { // Assuming there are at least 15 columns in each line
      string openingName = columns[14]; // "opening_name" column (index 14)
      // Perform case-insensitive comparison with user input opening
      if (openingName.compare(0, userInputOpening.size(), userInputOpening) ==
          0) {
        // If ':' is encountered in the opening name, use the full string as the
        // key
        size_t colonPos = openingName.find(':');
        string key =
            (colonPos != string::npos) ? openingName : userInputOpening;
        // Increment the count for this opening variation
        openingCounts[key]++;
      }
    }
  }
  file.close();

  // Convert the unordered_map to a vector of pairs for sorting
  vector<pair<string, int>> countsVector(openingCounts.begin(),
                                         openingCounts.end());

  // Sort the opening variations based on their counts in descending order
  mergeSort_for_variations(countsVector, 0, countsVector.size() - 1);

  // Print the counts of opening variations
  cout << "Opening Variation"
       << "   No of games in dataset" << endl;
  for (const auto &pair : countsVector) {
    cout << pair.first << " " << pair.second << endl;
  }
}


void add_game_to_csv(const string &filename, const vector<string> &gameData)
{
  ofstream file(filename, ios_base::app); // Open the file in append mode
  if (file.is_open()) {
    // Format the game data as a CSV string
    string gameString;
    for (size_t i = 0; i < gameData.size(); ++i) {
      gameString += gameData[i];
      if (i < gameData.size() - 1)
        gameString += ",";
    }
    file << endl << gameString;
    file.close();
    cout << "New game added to " << filename << " successfully." << endl;
  } else {
    cerr << "Unable to open file: " << filename << endl;
  }
}

// Function to get the game details from user input
vector<string> get_game_details()
{
  vector<string> gameData;
  string input;

  cout << "Enter game ID: ";
  getline(cin, input);
  gameData.push_back(input);

  cout << "Enter whether the game is rated (TRUE/FALSE): ";
  getline(cin, input);
  gameData.push_back(input);

  cout << "Enter created_at timestamp: ";
  getline(cin, input);
  gameData.push_back(input);

  cout << "Enter last_move_at timestamp: ";
  getline(cin, input);
  gameData.push_back(input);

  cout << "Enter number of turns: ";
  getline(cin, input);
  gameData.push_back(input);

  cout << "Enter victory status (mate/draw/resign): ";
  getline(cin, input);
  gameData.push_back(input);

  cout << "Enter result (white/draw/black): ";
  getline(cin, input);
  gameData.push_back(input);

  cout << "Enter increment code (Eg: 10+5): ";
  getline(cin, input);
  gameData.push_back(input);

  cout << "Enter white ID: ";
  getline(cin, input);
  gameData.push_back(input);

  cout << "Enter white rating: ";
  getline(cin, input);
  gameData.push_back(input);

  cout << "Enter black ID: ";
  getline(cin, input);
  gameData.push_back(input);

  cout << "Enter black rating: ";
  getline(cin, input);
  gameData.push_back(input);

  cout << "Enter moves: ";
  getline(cin, input);
  gameData.push_back(input);

  cout << "Enter opening eco: ";
  getline(cin, input);
  gameData.push_back(input);

  cout << "Enter opening name: ";
  getline(cin, input);
  gameData.push_back(input);

  cout << "Enter opening ply: ";
  getline(cin, input);
  gameData.push_back(input);

  return gameData;
}

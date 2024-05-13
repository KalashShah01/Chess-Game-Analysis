# Chess-Game-Analysis
This code tries to provide a certain analysis of the data set taken from Kaggle: https://www.kaggle.com/datasets/datasnaek/chess?select=games.csv

---
#### Collaborators : Maanya DN, Nidhi & Kalash Shah
---
#### The DataSet
The data set is stored in a CSV (Comma Separated Values) file and the code is written accordingly. The data set has a total of 16 collumns. Each collumns containing different information, for example, the 13th column has all the games moves are stored, separated by spaces. The 


---
### Functionality:
the code provides a certain functionality:
1. User inquiries & Actions
2. Opening Inquiries
3. Play A game

The explanations of all the functionality are written below.

---

#### 1. User Inquiries & Actions


**i. User Inquiries**: Enter a user ID to analyze statistics (wins, losses).
   - The algorithm will look at the data set and calculate from all the games played by the specific and print the information.

**ii. Top 3 Users:** View the top 3 users based on win-loss record and number of games.
   - The algorithm will look at the data set and store all the user occurences in an Unordered map (Hash table), and then find the best top three out of them, according to win over loss and draw ratio, and the number of games played ratio.

**iii. Add Game:** Add a new game to the dataset.
   - The code will manipulate the dataset by adding your own game.

---

#### 2. Opening Inquiries

**i. Sub Opening Name**: Users can input an opening name to retrieve sub-variation names along with the number of games found in the data set.

   - The algorithm finds all the games with the same first name and prints all the different strings found with the same first name.
   

**ii. Opening Moves**: Users can input an opening name to retrieve the opening moves associated with it.

- The 16th column, titled:  Opening Ply, contains how many moves are the opening theory in that particular game with that opening name. So the algorithm will search a row with that opening name and print the moves till the number Opening Ply suggests.


**iii. Identify Opening Name**: Users can input a series of moves to identify the most common opening name associated with those moves.
   - The algorithm will take the string of the moves, and find a game accordingly, and print from the column which corresponds to the game.

---

#### 3. Play A Game

**i. Play a 2-Player Game**:  Users can play a two-player game with each move. After each move, win, loss, and draw probabilities are displayed.

   - Code:

The first iteration will copy only the rows which contain the move provided by the user, the iteration is in the CSV file (the main data set), and at the end it will have a vector<vector<strings>> containing all the rows with all the columns.

A one-time merge sort is used to sort the games based on their moves, to increase the time for the next search.

All the next moves provided are found through binary search, and each time a new move is found it removes the games with other moves.

In each iteration, whenever a relevant game is found, it adds the win, draw and loss counts, and in the end, calculates and prints the probability.

2. **Play Against Computer**: Users can choose to play as white or black against the computer. The computer calculates and plays the best next move, also it will print the most played moves to aid the user.

- User Chooses Black: A first traverse will be initiated through each game row (in the CSV), and the first move string in an unordered map. Objects are stored with, one string (move) and three integers (black win, white win, draw, and occurrences) for each move. Each time the same move occurs again, the integer values will be increased appropriately. At the end, it will through all the unique moves and find and print the best move (considering the probability and the move occurrence) for white.
  
- User Chooses White: Here the first move will be given by the user, therefore, an iteration will be made in the entire CSV file and a vector with all the games starting from the move given will be created.
The next move in each step was stored in an unordered map, which had the structure of one string (move) and three integers (white win, black win and draw counts

- After both the first iterations, the same function by both processes will be called, which will operate on a vector, and it will keep on finding the best moves for the computer, and print the top five occurrences of the next moves (for the user), and the loop will just append the user's move the game moves string.

---

#### Additional Features

- **Data Handling**:
  - Functions to read from and write to a CSV file containing chess game data.
  - Manipulation of game data such as sorting, searching, and filtering.

- **String Manipulation**:
  - Functions to handle string operations such as splitting, extraction, and manipulation.

- **User Interface**:
  - Clearing the screen for better user experience.
  - Error handling for invalid input.

---

#### How to Use:
Use: "make" to compile the code in your shell.
```
  make
  ./my_chess #To run the code. 
```
- Upon running, users are presented with a main menu.
- One can choose from different options such as User Inquiries & Actions, Opening Inquiries, Play Game, or Exit.
- Depending on the selected option, users can input further details or commands to perform specific actions.

---

#### Dependencies:

- The code utilizes standard C++ libraries for file handling, string manipulation, and user input/output.
- No external libraries or dependencies are required.

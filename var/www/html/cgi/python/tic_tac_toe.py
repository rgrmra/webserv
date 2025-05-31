import os
import urllib.parse

PLAYER_X = 'close'
PLAYER_O = 'circle'
EMPTY = ''

content_length = int(os.environ.get('CONTENT_LENGTH', 0))

post_data = os.read(0, content_length).decode('utf-8')

data = urllib.parse.parse_qs(post_data)

current_board = data.get('board', [',,,,,,,,'])[0].split(',')
current_player = data.get('current_player', [PLAYER_X])[0]

if data.get('restart'):
    current_board = [EMPTY] * 9
    current_player = PLAYER_X
else:
    move = data.get('move', [None])[0]
    if move and move.isdigit():
        move = int(move)
        if 0 <= move < 9 and current_board[move] == EMPTY:
            current_board[move] = current_player
            current_player = PLAYER_O if current_player == PLAYER_X else PLAYER_X

def check_winner(board):
    lines = [
        [0, 1, 2], [3, 4, 5], [6, 7, 8],
        [0, 3, 6], [1, 4, 7], [2, 5, 8],
        [0, 4, 8], [2, 4, 6]
    ]

    for a, b, c in lines:
        if board[a] and board[a] == board[b] == board[c]:
            return board[a]

    return None

player = lambda x: "X" if x == PLAYER_X else "O"

def get_message():
    if winner:
        return f'Player {player(winner)} wins!'
    elif tie:
        return f'Game is a tie!'
    else:
        return f'Current Player: {player(current_player)}'

def get_board():
    board: str = ''
    for row in range(3):
        board += '<tr>'
        for col in range(3):
            cell = row * 3 + col
            value = current_board[cell]
            if value == '' and not winner and not tie:
                board += f'<td><button type="submit" name="move" value="{cell}">&nbsp;</button></td>'
            else:
                board += f'<td><span class="material-symbols-outlined">{value if value else "&nbsp;"}</span></td>'
        board += '</tr>'
    return board

winner = check_winner(current_board)
tie = not winner and '' not in current_board

print("Content-Type: text/html\n")
print(f"""
<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <title>Tic Tac Toe</title>
        <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300;400;700&display=swap" rel="stylesheet" />
        <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined"/>
        <link href="/cgi/library/css/layout.css" rel="stylesheet">
    </head>
    <body>
        <h1>Tic Tac Toe</h1>
        <div class="container">
            <div class="test-section">
                <h2>Play Tic Tac Toe</h2>
                <div class="message">{get_message()}</div>
                <form class="form-group" method="POST">
                    <input type="hidden" name="board" value="{','.join(current_board)}">
                    <input type="hidden" name="current_player" value="{current_player}">
                    <table>{get_board()}</table>
                </form>
                <form class="form-group" method="POST" class="restart-form">
                    <input type="submit" name="restart" value="Restart Game">
                </form>
                <a href="/cgi/index.html#tic_tac_toe">Back to Tests</a>
            </div>
        </div>
    </body>
</html>
""")

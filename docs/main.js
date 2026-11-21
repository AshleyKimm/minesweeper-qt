// UI — a port of mainwindow.cpp.

const COLOURS = ["#302151", "#5C2247", "#89233E", "#B52434", "#B52434",
                 "#CC2530", "#E2252B", "#F82626", "#E80409"];

const $ = (id) => document.getElementById(id);

let xdim = 0, ydim = 0, numMine = 0;
let board = null;
let cells = [];
let gameOver = false;
let flagMode = false;

$("setup-form").addEventListener("submit", (e) => {
  e.preventDefault();
  const rows = parseInt($("rows").value, 10);
  const cols = parseInt($("cols").value, 10);
  const mines = parseInt($("mines").value, 10);

  const valid = [rows, cols, mines].every(Number.isInteger) &&
    Math.min(rows, cols) > 0 && Math.max(rows, cols) <= 25 &&
    mines > 0 && mines <= rows * cols;
  $("setup-error").hidden = valid;
  if (!valid) return;

  xdim = cols; ydim = rows; numMine = mines;
  $("setup").hidden = true;
  $("game").hidden = false;
  newGame();
});

$("restart").addEventListener("click", newGame);
$("back").addEventListener("click", () => {
  $("game").hidden = true;
  $("setup").hidden = false;
});
$("flag-mode").addEventListener("click", () => {
  flagMode = !flagMode;
  $("flag-mode").setAttribute("aria-pressed", flagMode);
  $("flag-mode").textContent = `🚩 Flag mode: ${flagMode ? "on" : "off"}`;
});

function newGame() {
  board = startGame(xdim, ydim, numMine);
  gameOver = false;
  $("status").textContent = "";

  const grid = $("board");
  grid.innerHTML = "";
  grid.style.setProperty("--cols", xdim);
  cells = [];
  for (let y = 0; y < ydim; y++) {
    for (let x = 0; x < xdim; x++) {
      const btn = document.createElement("button");
      btn.className = "cell";
      btn.type = "button";
      btn.addEventListener("click", () => (flagMode ? cellFlag(x, y) : cellReveal(x, y)));
      btn.addEventListener("contextmenu", (e) => { e.preventDefault(); cellFlag(x, y); });
      grid.appendChild(btn);
      cells.push(btn);
    }
  }
  render();
}

function cellFlag(x, y) {
  if (gameOver) return;
  mark(board, xdim, x, y);
  render();
}

function cellReveal(x, y) {
  if (gameOver) return;
  const result = reveal(board, xdim, ydim, x, y);
  if (result === MINE) {
    gameOver = true;
    // Show every mine so the player can see what they missed.
    for (let i = 0; i < board.length; i++) {
      if ((board[i] & VALUE_MASK) === MINE) board[i] &= ~(HIDDEN_BIT | MARKED_BIT);
    }
    $("status").textContent = "You lost! You clicked a mine!";
  } else if (isGameWon(board)) {
    gameOver = true;
    $("status").textContent = "Victory! You revealed all safe cells!";
  }
  render();
}

function render() {
  let flags = 0;
  for (let i = 0; i < board.length; i++) {
    const cell = cells[i];
    const v = board[i];
    cell.className = "cell";
    cell.style.backgroundColor = "";
    if (v & MARKED_BIT) flags++;

    if (v & HIDDEN_BIT) {
      cell.textContent = v & MARKED_BIT ? "🚩" : "";
      cell.classList.add("hidden");
    } else if ((v & VALUE_MASK) === MINE) {
      cell.textContent = "💣";
      cell.classList.add("mine");
    } else {
      const n = v & VALUE_MASK;
      cell.textContent = n === 0 ? "" : n;
      cell.style.backgroundColor = COLOURS[n];
    }
  }
  $("mine-count").textContent = `💣 = ${numMine - flags}`;
  $("board").classList.toggle("over", gameOver);
}

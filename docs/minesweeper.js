// Game logic — a port of minesweeper.cpp / minesweeper_lib.cpp.
// Each cell is one byte, same encoding as the C++ version:
//   bits 0-3 (valueMask)  : 0-8 = neighbouring mines, 9 = mine
//   bit 4    (markedBit)  : cell is flagged
//   bit 5    (hiddenBit)  : cell is still hidden

const MARKED_BIT = 0x10;
const HIDDEN_BIT = 0x20;
const VALUE_MASK = 0x0f;
const MINE = 9;

function createBoard(xdim, ydim) {
  return new Uint8Array(xdim * ydim);
}

function spreadMine(board, numMine) {
  for (let placed = 0; placed < numMine; placed++) {
    let pos;
    do {
      pos = Math.floor(Math.random() * board.length);
    } while (board[pos] !== 0);
    board[pos] = MINE;
  }
}

// Calls fn(nx, ny, index) for every in-bounds neighbour of (x, y).
function forEachNeighbor(xdim, ydim, x, y, fn) {
  for (let dy = -1; dy <= 1; dy++) {
    for (let dx = -1; dx <= 1; dx++) {
      if (dx === 0 && dy === 0) continue;
      const nx = x + dx, ny = y + dy;
      if (nx >= 0 && nx < xdim && ny >= 0 && ny < ydim) fn(nx, ny, ny * xdim + nx);
    }
  }
}

function computeNeighbors(board, xdim, ydim) {
  for (let i = 0; i < board.length; i++) {
    if ((board[i] & VALUE_MASK) !== MINE) continue;
    forEachNeighbor(xdim, ydim, i % xdim, Math.floor(i / xdim), (nx, ny, n) => {
      if ((board[n] & VALUE_MASK) !== MINE) board[n]++;
    });
  }
}

function hideBoard(board) {
  for (let i = 0; i < board.length; i++) board[i] |= HIDDEN_BIT;
}

function startGame(xdim, ydim, numMine) {
  const board = createBoard(xdim, ydim);
  spreadMine(board, numMine);
  computeNeighbors(board, xdim, ydim);
  hideBoard(board);
  return board;
}

// Returns 2 if the cell is already revealed, 1 if it was unflagged, 0 if it was flagged.
function mark(board, xdim, x, y) {
  const i = y * xdim + x;
  if (!(board[i] & HIDDEN_BIT)) return 2;
  board[i] ^= MARKED_BIT;
  return board[i] & MARKED_BIT ? 0 : 1;
}

// Returns 1 if flagged, 2 if already revealed, 9 if a mine was hit, 0 otherwise.
// Revealing a 0 cascades outward until it hits numbered cells.
function reveal(board, xdim, ydim, x, y) {
  const i = y * xdim + x;
  if (board[i] & MARKED_BIT) return 1;
  if (!(board[i] & HIDDEN_BIT)) return 2;

  board[i] &= ~HIDDEN_BIT;
  if ((board[i] & VALUE_MASK) === MINE) return MINE;

  const stack = (board[i] & VALUE_MASK) === 0 ? [[x, y]] : [];
  while (stack.length) {
    const [cx, cy] = stack.pop();
    forEachNeighbor(xdim, ydim, cx, cy, (nx, ny, n) => {
      if (!(board[n] & HIDDEN_BIT) || (board[n] & MARKED_BIT)) return;
      board[n] &= ~HIDDEN_BIT;
      if ((board[n] & VALUE_MASK) === 0) stack.push([nx, ny]);
    });
  }
  return 0;
}

function isGameWon(board) {
  for (let i = 0; i < board.length; i++) {
    if ((board[i] & HIDDEN_BIT) && (board[i] & VALUE_MASK) !== MINE) return false;
  }
  return true;
}

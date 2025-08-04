# Source Code

<details>
  <summary>Click to expand the full Flutter Snake Game code 🚀</summary>

```dart
import 'dart:math';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter/scheduler.dart';

void main() {
  runApp(const SnakeGameApp());
}

class SnakeGameApp extends StatelessWidget {
  const SnakeGameApp({super.key});
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Snake Game',
      theme: ThemeData.dark(),
      home: const WelcomeScreen(),
      debugShowCheckedModeBanner: false,
    );
  }
}

class WelcomeScreen extends StatelessWidget {
  const WelcomeScreen({super.key});
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.yellow.shade900,
      body: SafeArea(
        child: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              const Text(
                "Swagat xw Snake Game Ma❤",
                style: TextStyle(fontSize: 30, fontWeight: FontWeight.bold, color: Colors.purple),
                textAlign: TextAlign.center,
              ),
              const SizedBox(height: 20),
              const Text(
                "Designed by Anup Raj Thapaliya😁🥳",
                style: TextStyle(fontSize: 32, fontWeight: FontWeight.bold, color: Colors.white),
                textAlign: TextAlign.center,
              ),
              const SizedBox(height: 42),
              ElevatedButton(
                onPressed: () {
                  Navigator.pushReplacement(
                    context,
                    MaterialPageRoute(builder: (context) => const SnakeGame()),
                  );
                },
                child: const Text("Yeta Click👀"),
              ),
            ],
          ),
        ),
      ),
    );
  }
}

enum Direction { up, down, left, right }

class SnakeGame extends StatefulWidget {
  const SnakeGame({super.key});
  @override
  State<SnakeGame> createState() => _SnakeGameState();
}

class _SnakeGameState extends State<SnakeGame> with SingleTickerProviderStateMixin {
  static const int rows = 20;
  static const int cols = 20;
  static const Duration baseSpeed = Duration(milliseconds: 200);
  late Ticker _ticker;
  Duration _elapsed = Duration.zero;
  Duration _moveInterval = baseSpeed;
  final List<Point<int>> _snake = [const Point(10, 10)];
  late Point<int> _food;
  Direction _direction = Direction.right;
  bool _isGameOver = false;
  bool _isPaused = false;
  int _score = 0;
  int _highScore = 0;
  final FocusNode _focusNode = FocusNode();

  @override
  void initState() {
    super.initState();
    _generateFood();
    _ticker = createTicker(_onTick)..start();
    WidgetsBinding.instance.addPostFrameCallback((_) {
      _focusNode.requestFocus();
    });
  }

  void _onTick(Duration elapsed) {
    if (_isGameOver || _isPaused) return;
    if (elapsed - _elapsed >= _moveInterval) {
      _elapsed = elapsed;
      _moveSnake();
    }
  }

  void _generateFood() {
    final rand = Random();
    do {
      _food = Point(rand.nextInt(cols), rand.nextInt(rows));
    } while (_snake.contains(_food));
  }

  void _moveSnake() {
    final head = _snake.first;
    Point<int> newHead;
    switch (_direction) {
      case Direction.up:
        newHead = Point(head.x, head.y - 1);
        break;
      case Direction.down:
        newHead = Point(head.x, head.y + 1);
        break;
      case Direction.left:
        newHead = Point(head.x - 1, head.y);
        break;
      case Direction.right:
        newHead = Point(head.x + 1, head.y);
        break;
    }

    if (newHead.x < 0 || newHead.y < 0 || newHead.x >= cols || newHead.y >= rows || _snake.contains(newHead)) {
      setState(() {
        _isGameOver = true;
        if (_score > _highScore) _highScore = _score;
      });
      return;
    }

    setState(() {
      _snake.insert(0, newHead);
      if (newHead == _food) {
        _score++;
        _generateFood();
        _moveInterval = Duration(milliseconds: (_moveInterval.inMilliseconds * 0.985).toInt());
      } else {
        _snake.removeLast();
      }
    });
  }

  void _changeDirection(Direction newDirection) {
    if ((_direction == Direction.up && newDirection == Direction.down) ||
        (_direction == Direction.down && newDirection == Direction.up) ||
        (_direction == Direction.left && newDirection == Direction.right) ||
        (_direction == Direction.right && newDirection == Direction.left)) return;
    setState(() {
      _direction = newDirection;
    });
  }

  void _restart() {
    setState(() {
      _snake.clear();
      _snake.add(const Point(10, 10));
      _direction = Direction.right;
      _isGameOver = false;
      _score = 0;
      _isPaused = false;
      _moveInterval = baseSpeed;
      _generateFood();
    });
  }

  void _togglePause() {
    setState(() {
      _isPaused = !_isPaused;
    });
  }

  @override
  void dispose() {
    _ticker.dispose();
    _focusNode.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.green.shade800,
      body: SafeArea(
        child: Column(
          children: [
            const Padding(
              padding: EdgeInsets.all(16),
              child: Text("Feed the🐍 ", style: TextStyle(fontSize: 30, fontWeight: FontWeight.bold)),
            ),
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                Text("Current Score👇: $_score", style: const TextStyle(fontSize: 15)),
                const SizedBox(width: 20),
                Text("High Score🥶: $_highScore", style: const TextStyle(fontSize: 15)),
              ],
            ),
            const SizedBox(height: 16),
            Expanded(
              child: AspectRatio(
                aspectRatio: cols / rows,
                child: RawKeyboardListener(
                  focusNode: _focusNode,
                  onKey: (event) {
                    if (event is RawKeyDownEvent) {
                      switch (event.logicalKey.keyLabel) {
                        case 'Arrow Up':
                        case 'w':
                          _changeDirection(Direction.up);
                          break;
                        case 'Arrow Down':
                        case 's':
                          _changeDirection(Direction.down);
                          break;
                        case 'Arrow Left':
                        case 'a':
                          _changeDirection(Direction.left);
                          break;
                        case 'Arrow Right':
                        case 'd':
                          _changeDirection(Direction.right);
                          break;
                      }
                    }
                  },
                  child: GestureDetector(
                    onVerticalDragUpdate: (details) {
                      if (details.primaryDelta! < 0) _changeDirection(Direction.up);
                      if (details.primaryDelta! > 0) _changeDirection(Direction.down);
                    },
                    onHorizontalDragUpdate: (details) {
                      if (details.primaryDelta! < 0) _changeDirection(Direction.left);
                      if (details.primaryDelta! > 0) _changeDirection(Direction.right);
                    },
                    child: Container(
                      decoration: BoxDecoration(
                        color: Colors.green.shade900,
                        borderRadius: BorderRadius.circular(12),
                      ),
                      child: CustomPaint(
                        painter: SnakePainter(_snake, _food, rows, cols),
                      ),
                    ),
                  ),
                ),
              ),
            ),
            const SizedBox(height: 16),
            if (!_isGameOver)
              ElevatedButton(
                onPressed: _togglePause,
                child: Text(_isPaused ? "Start Snake🧐" : "Stop Snake😏"),
              ),
            if (_isGameOver)
              ElevatedButton(
                onPressed: _restart,
                child: const Text("TryAgain😤"),
              ),
            const SizedBox(height: 20),
          ],
        ),
      ),
    );
  }
}

class SnakePainter extends CustomPainter {
  final List<Point<int>> snake;
  final Point<int> food;
  final int rows, cols;

  SnakePainter(this.snake, this.food, this.rows, this.cols);

  @override
  void paint(Canvas canvas, Size size) {
    final tileW = size.width / cols;
    final tileH = size.height / rows;
    final snakePaint = Paint()..color = Colors.pinkAccent;

    for (final s in snake) {
      final radius = tileW * 0.3;
      canvas.drawRRect(
        RRect.fromRectAndRadius(
          Rect.fromLTWH(s.x * tileW, s.y * tileH, tileW, tileH),
          Radius.circular(radius),
        ),
        snakePaint,
      );
    }

    final foodPaint = Paint()..color = Colors.red;
    canvas.drawOval(
      Rect.fromLTWH(food.x * tileW, food.y * tileH, tileW, tileH),
      foodPaint,
    );

    final head = snake.first;
    final headPaint = Paint()..color = Colors.brown;
    canvas.drawCircle(
      Offset(head.x * tileW + tileW / 2, head.y * tileH + tileH / 2),
      tileW * 0.2,
      headPaint,
    );
  }

  @override
  bool shouldRepaint(CustomPainter oldDelegate) => true;
}

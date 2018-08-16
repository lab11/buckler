Yakindu example app
===================

Example of using [Yakindu statecharts](https://www.itemis.com/en/yakindu/state-machine/)
in an application. Run `make rtt` to see output

State machine:
```
 Entry
   |
 always
   |
+--------+  --tick-->  +--------+
| x += 1 |             | y += 1 |
+--------+  <--tick--  +--------+
   |
 x+y >= 6
   |
 Exit
```

Example output:
```
Log initialized!
Iteration: 0
Count: 0
Point.x: 0	Point.y: 0
Iteration: 1
Count: 1
Point.x: 0	Point.y: 1
Iteration: 2
Count: 2
Point.x: 1	Point.y: 1
Iteration: 3
Count: 3
Point.x: 1	Point.y: 2
Iteration: 4
Count: 4
Point.x: 2	Point.y: 2
Iteration: 5
Count: 5
Point.x: 2	Point.y: 3
Iteration: 6
Count: 6
Point.x: 3	Point.y: 3
Iteration: 7
Count: 6
Point.x: 3	Point.y: 3
...
```


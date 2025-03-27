# Project-for-Operating-Systems-Course
Repository contains first project for OS course which is solution for Dining philosophers problem in [C](https://programmerhumor.io/programming-memes/c-programming/).
# Resources
- [Dining philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [pthread_create](https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread_create.html)
- [usleep](https://www.man7.org/linux/man-pages/man3/usleep.3.html)
- [pthread_cond](https://docs.oracle.com/cd/E19455-01/806-5257/6je9h032r/index.html)

# Usage

```
cd Operating-Systems-Project
cmake -B out
cmake --build out
./out/dpp <number_of_philosophers>
```

To end program you will have use ```Ctrl^C```  (SIGINT), it gives summary of how long each philosopher ate and exits program.



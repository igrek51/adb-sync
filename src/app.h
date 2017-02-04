#ifndef APP_H
#define APP_H

class App;

class App {
private:
    bool _exit;
public:
    App();

    ~App();


    // czy należy zamknąć aplikację
    bool exiting();

    /// zgłoszenie sygnału zakończenia aplikacji
    void exit();
};

#endif

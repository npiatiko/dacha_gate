class gateController {
   private:
    gateController(const gateController&) = delete;
    gateController& operator=(gateController&) = delete;
    gateController();

   public:
    static gateController& getController();
    static void intrptHndlr();
    void openGate();
    void closeGate();

    ~gateController() = default;
};

//
// Created by Zachary Halpern on 5/11/18.
//

#ifndef ASSIGNMENT4_FINAL_H
#define ASSIGNMENT4_FINAL_H

/**
 * A banking system needs
 * A way to login/access your information
 * A way to deposit/withdraw funds
 * A way to create/delete accounts
 */
class Bank
{
    private:
        std::vector<Account> bank_accounts;

    public:
        /**
         * No default banks
         */
        Bank() = delete;

        /**
         * Name the bank
         * @param bank_name
         */
        Bank(const std::string bank_name);

        /**
         * If we need to transition the bank
         * @param old
         */
        Bank(const Bank &old);

        /**
         * If we need to make a new bank using the same stats
         * @param copy
         * @return
         */
        Bank& operator=(const Bank &copy);

        /**
         * Bankruptcy :(
         */
        ~Bank();

        /**
         * Add a new account to our bank and include
         * how much interest this specific account
         * gets, how often to compound, and how
         * much was the initial deposit by the user.
         * In addition, a way for the user to login
         * @param name
         * @param password
         * @param initial_deposit >= 0
         * @param interest_rate >= 0
         * @param compounds_per_yr >=0
         * @return Account is made or not
         */
        bool addNewAccount(std::string name, std::string password, float initial_deposit, float interest_rate, int compounds_per_yr);

        /**
         * Validate the user's login
         * and give them a session_key
         * for critical user functions
         * @param name
         * @param hashed_password
         * @return session_key
         */
        std::string loginToViewInformation(std::string name, std::string hashed_password);

        /**
         * Delete the user's account
         * Validate the session_key
         * @param name
         * @param session_key
         * @return
         */
        bool deleteAccount(std::string name, std::string session_key);

        /**
         * Withdraw funds from the user account
         * This will return the amount withdrawn with the remaining balance
         * if the user has enough funds, otherwise it will return
         * a pair of {0, balance}.
         * @param amount_to_withdraw
         * @param session_key
         * @return {Amount withdrawn, remaining balance}
         */
        std::pair<float, float> withdrawFunds(float amount_to_withdraw, std::string session_key);

        /**
         * Add funds to the user account
         * This will return the amount added and the new balance
         * @param amount_to_add
         * @param session_key
         * @return
         */
        std::pair<float, float> addFunds(float amount_to_add, std::string session_key);

        /**
         * A way for the user to change their password
         * Returns if they were successful in the change or not
         * @param old_hashed_password
         * @param new_hashed_password
         * @param session_key
         * @return
         */
        bool changeAccountPassword(std::string old_hashed_password, std::string new_hashed_password, std::string session_key);
};

class Account
{
    private:
        std::string name, hashed_password;
        float balance, interest_rate;
        int compounds_per_yr;

    public:
        /**
         * All accounts require a name and password, at minimum
         */
        Account() = delete;

        /**
         * Create the account, storing all necessary information for this account
         * In addition, hash the password submitted by the user for future verifications
         * @param name
         * @param password
         * @param init_depo
         * @param int_rate
         * @param compounds_per_yr
         */
        Account(std::string name, std::string password, float init_depo = 0, float int_rate = 0, int compounds_per_yr = 0);

        /**
         * Copy constructor
         * @param old
         */
        Account(const Account &old);

        /**
         * Closing up shop for this account
         */
        ~Account();
};

/* Free Standing Bank */
struct free_banks
{
    /**
     * Name of the bank
     */
    std::string name;

    /**
     * Vector of accounts
     * Account: {name, hashed_pw, balance, interest_rate, compounds_per_yr}
     */
    std::vector<std::tuple<std::string, std::string, float, float, int> > accounts;
};

std::vector<free_banks> global_bank_list;

/**
 * Creates a bank, puts it into the global_bank_list
 * and returns the index at which it can be found
 * @param bank_name
 * @return
 */
int createNewBank(std::string bank_name);

/**
 * Adds a new account to the bank at index bank_id
 * @param bank_id
 * @param name
 * @param password
 * @param initial_deposit
 * @param interest_rate
 * @param compounds_per_yr
 * @return
 */
bool addNewAccount(int bank_id, std::string name, std::string password, float initial_deposit, float interest_rate, int compounds_per_yr);

/**
 * Delete an account from a certain bank,
 * provided the credentials are valid
 * @param bank_id
 * @param name
 * @param hashed_password
 * @return
 */
bool deleteAccount(int bank_id, std::string name, std::string hashed_password);

/**
 * Withdraw funds from a certain bank
 * account, provided credentials are valid
 * while also preventing overdraws.
 * @param bank_id
 * @param amount_to_withdraw
 * @param session_key
 * @return
 */
std::pair<float, float> withdrawFunds(int bank_id, float amount_to_withdraw, std::string name, std::string hashed_password);

/**
 * Add funds to a certain bank
 * account, provided credentials are valid.
 * @param amount_to_add
 * @param name
 * @param hashed_password
 * @return
 */
std::pair<float, float> addFunds(float amount_to_add, std::string name, std::string hashed_password);

/**
 * Change password for a certain user
 * in a certain bank.
 * @param bank_id
 * @param name
 * @param old_hashed_password
 * @param new_hashed_password
 * @return
 */
bool changeAccountPassword(int bank_id, std::string name, std::string old_hashed_password, std::string new_hashed_password);

/* Max functions */
// For loop implementation
int max(int *array, int array_size)
{
    if (array_size <= 0)
    {
        throw arraySizeInvalid();
    }

    int max_value = array[0];
    for (int i = 1; i < array_size; i++)
    {
        if (array[i] > max_value)
        {
            max_value = array[i];
        }
    }

    return max_value;
}

int max(int *array, int array_size)
{
    if (array_size <= 0)
    {
        return -infinity;
    }

    const int curr_val = array[array_size - 1];

    if (array_size == 1)
    {
        return curr_val;
    }

    const int max_val = max(array, array_size - 1);

    return (curr_val < max_val) ? max_val : curr_val;
}

#endif //ASSIGNMENT4_FINAL_H

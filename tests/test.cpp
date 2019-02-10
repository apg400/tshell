#include "../src/command.h"
#include "../src/operator.h"
#include "../src/parser.h"
#include "../src/testcmd.h"
#include "../src/pipe.h"
#include "../src/redirect.h"

#include <string.h>
#include <unistd.h>
#include <iostream>
#include "gtest/gtest.h"

const char* w1 = "echo";
const char* w2 = "hello";
const char* w3 = "";
const char* w4 = "kill me";

TEST(SingleCommand, Compiles) {
    std::string input = "echo Hello";
    Parser* parser = new Parser((char*)input.c_str()) ;
    Action* root = parser->generateExecTree();
    EXPECT_EQ(true, root->execute(0, 1));

}

TEST(Parser, ParseHello) {
    std::vector<char*> vect = {(char*) w1, (char*) w2};
    std::string input = "echo hello";
    Parser* parser = new Parser((char*)input.c_str());
    std::vector<char*> temp = parser->parse();

    EXPECT_EQ(*vect[0],*temp[0]);
    EXPECT_EQ(*vect[1], *temp[1]);
}

TEST(Command, SingleCommand) {
    std::vector<char*> vect = {(char*) w1, (char*) w3};
    Command* command = new Command(vect);
    bool temp = command->execute(0, 1);
    EXPECT_EQ(true, temp);
}

TEST(AND, TwoCommands) {
    std::vector<char*> vect1 = {(char*) w1, (char*) w3};
    Command* command = new Command(vect1);
    std:: vector<char*> vect2 = {(char*) w1, (char*) w3};
    Command* command1 = new Command(vect1);
    Command* command2 = new Command(vect2);
    And testAnd = And(command1, command2);
    bool comparison = testAnd.execute(0, 1);
    EXPECT_EQ(true, comparison);
}

TEST(OR, TwoCommands) {
    std::vector<char*> vect1 = {(char*) w1, (char*) w3};
    Command* command = new Command(vect1);
    std:: vector<char*> vect2 = {(char*) w1, (char*) w3};
    Command* command1 = new Command(vect1);
    Command* command2 = new Command(vect2);
    Or testOr = Or(command1, command2);
    bool comparison = testOr.execute(0, 1);
    EXPECT_EQ(true, comparison);
}

TEST(CONNECT, TwoCommands) {
    std::vector<char*> vect1 = {(char*) w1, (char*) w3};
    Command* command = new Command(vect1);
    std:: vector<char*> vect2 = {(char*) w1, (char*) w3};
    Command* command1 = new Command(vect1);
    Command* command2 = new Command(vect2);
    Connect testConnect = Connect(command1, command2);
    bool comparison = testConnect.execute(0, 1);
    EXPECT_EQ(true, comparison);
}

TEST(TESTCOMMAND, NoFlag_Directory) {
    const char* test = "test";
    const char* pathway = "src";
    std::vector<char*> vect = {(char*) test, (char*) pathway};
    TestCMD testCommand = TestCMD(vect);
    bool comparison = testCommand.execute(0, 1);
    EXPECT_EQ(true, comparison);
}

TEST(TESTCOMMAND, NoFlag_File) {
    const char* test = "test";
    const char* pathway = "CMakeLists.txt";
    std::vector<char*> vect = {(char*) test, (char*) pathway};
    TestCMD testCommand = TestCMD(vect);
    bool comparison = testCommand.execute(0, 1);
    EXPECT_EQ(true, comparison);
}

TEST(TESTCOMMAND, DirectoryFlag) {
    const char* test = "test";
    const char* flag = "-d";
    const char* pathway = "src";
    std::vector<char*> vect = {(char*) test,(char*) flag, (char*) pathway};
    TestCMD testCommand = TestCMD(vect);
    bool comparison = testCommand.execute(0, 1);
    EXPECT_EQ(true, comparison);
}

TEST(TESTCOMMAND, FileFlag) {
    const char* test = "test";
    const char* flag = "-f";
    const char* pathway = "CMakeLists.txt";
    std::vector<char*> vect = {(char*) test,(char*) flag, (char*) pathway};
    TestCMD testCommand = TestCMD(vect);
    bool comparison = testCommand.execute(0, 1);
    EXPECT_EQ(true, comparison);
}

TEST(TESTCOMMAND, EverythingFlag_Directory) {
    const char* test = "test";
    const char* flag = "-e";
    const char* pathway = "src";
    std::vector<char*> vect = {(char*) test,(char*) flag, (char*) pathway};
    TestCMD testCommand = TestCMD(vect);
    bool comparison = testCommand.execute(0, 1);
    EXPECT_EQ(true, comparison);
}

TEST(TESTCOMMAND, EverythingFlag_File) {
    const char* test = "test";
    const char* flag = "-e";
    const char* pathway = "CMakeLists.txt";
    std::vector<char*> vect = {(char*) test,(char*) flag, (char*) pathway};
    TestCMD testCommand = TestCMD(vect);
    bool comparison = testCommand.execute(0, 1);
    EXPECT_EQ(true, comparison);
}

TEST(TESTCOMMAND, DirectoryFlag_Fail) {
    const char* test = "test";
    const char* flag = "-d";
    const char* pathway = "CMakeLists.txt";
    std::vector<char*> vect = {(char*) test,(char*) flag, (char*) pathway};
    TestCMD testCommand = TestCMD(vect);
    bool comparison = testCommand.execute(0, 1);
    EXPECT_EQ(false, comparison);
}

TEST(TESTCOMMAND, FileFlag_Fail) {
    const char* test = "test";
    const char* flag = "-f";
    const char* pathway = "src";
    std::vector<char*> vect = {(char*) test,(char*) flag, (char*) pathway};
    TestCMD testCommand = TestCMD(vect);
    bool comparison = testCommand.execute(0, 1);
    EXPECT_EQ(false, comparison);
}

TEST(REDIRECT, Output) {
   const char* list = "ls";
   const char* file = "output.txt";
   std::vector<char*> vect = {(char*) list};
   Command* testCommand = new Command(vect);
   RedirectOutput testOutput = RedirectOutput(testCommand, (char*) file);
   bool comparison = testOutput.execute(0,1);
   EXPECT_EQ(true, comparison);
   
}

TEST(REDIRECT, OutputFail) {
   const char* list = "ls";
   const char* file = "forceFail.txt";
   std::vector<char*> vect = {(char*) list};
   Command* testCommand = new Command(vect);
   RedirectOutput testOutput = RedirectOutput(NULL, NULL);
   bool comparison = testOutput.execute(0,1);
   EXPECT_EQ(false, comparison);
   
}

TEST(REDIRECT, Append) {
   const char* list = "ls";
   const char* file = "output.txt";
   std::vector<char*> vect = {(char*) list};
   Command* testCommand = new Command(vect);
   AppendOutput testAppend = AppendOutput(testCommand, (char*) file);
   bool comparison = testAppend.execute(0,1);
   EXPECT_EQ(true, comparison);
   
}

TEST(REDIRECT, Input) {
   const char* list = "ls";
   const char* file = "input.txt";
   std::vector<char*> vect = {(char*) list};
   Command* testCommand = new Command(vect);
   RedirectInput testInput = RedirectInput(testCommand, (char*) file);
   bool comparison = testInput.execute(0,1);
   EXPECT_EQ(false, comparison);
   
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


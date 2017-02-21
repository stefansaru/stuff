#include <iostream>

using namespace std;

// -- Rule 1: "682: One number is correct and well placed"
inline bool rule_1(int first, int second, int third)
{
  bool first_correct_and_well_placed = (first == 6) ? true : false;
  bool second_correct_and_well_placed = (second == 8) ? true : false;
  bool third_correct_and_well_placed = (third == 2) ? true : false;

  if  (
       (first_correct_and_well_placed == true  && second_correct_and_well_placed == false && third_correct_and_well_placed == false)
    || (first_correct_and_well_placed == false && second_correct_and_well_placed == true  && third_correct_and_well_placed == false)
    || (first_correct_and_well_placed == false && second_correct_and_well_placed == false && third_correct_and_well_placed == true )
  )
  {
    cout << first << second << third << ": rule 1: match" << endl;
    return true;
  }

  cout << first << second << third << ": rule 1: fail" << endl;
  return false;
}

// -- Rule 2: "614: One number is correct but wrong placed"
inline bool rule_2(int first, int second, int third)
{
  bool first_correct_but_wrong_placed = ((first == 1 || first == 4) && (first != 6)) ? true : false;
  bool second_correct_but_wrong_placed = ((second == 6 || second == 4) && (second != 1)) ? true : false;
  bool third_correct_but_wrong_placed = ((third == 6 || third == 1) && (third != 4)) ? true : false;

  if  (
       (first_correct_but_wrong_placed == true  && second_correct_but_wrong_placed == false && third_correct_but_wrong_placed == false)
    || (first_correct_but_wrong_placed == false && second_correct_but_wrong_placed == true  && third_correct_but_wrong_placed == false)
    || (first_correct_but_wrong_placed == false && second_correct_but_wrong_placed == false && third_correct_but_wrong_placed == true )
  )
  {
    cout << first << second << third << ": rule 2: match" << endl;
    return true;
  }

  cout << first << second << third << ": rule 2: fail" << endl;
  return false;
}

// -- Rule 3: "206: Two numbers are correct but wrong placed"
inline bool rule_3(int first, int second, int third)
{
  bool first_correct_but_wrong_placed = (second == 2 || third == 2) && first != 2;
  bool second_correct_but_wrong_placed = (first == 0 || third == 0) && second != 0;
  bool third_correct_but_wrong_placed = (first == 6 || second == 6) && third != 6;

  bool first_and_second_correct_but_wrong_placed = first_correct_but_wrong_placed && second_correct_but_wrong_placed;
  bool second_and_third_correct_but_wrong_placed = second_correct_but_wrong_placed && third_correct_but_wrong_placed;
  bool first_and_third_correct_but_wrong_placed = first_correct_but_wrong_placed && third_correct_but_wrong_placed;

  if  (
       (first_and_second_correct_but_wrong_placed == true  && second_and_third_correct_but_wrong_placed == false && first_and_third_correct_but_wrong_placed == false)
    || (first_and_second_correct_but_wrong_placed == false && second_and_third_correct_but_wrong_placed == true  && first_and_third_correct_but_wrong_placed == false)
    || (first_and_second_correct_but_wrong_placed == false && second_and_third_correct_but_wrong_placed == false && first_and_third_correct_but_wrong_placed == true )
  )
  {
    cout << first << second << third << ": rule 3: match" << endl;
    return true;
  }

  cout << first << second << third << ": rule 3: fail" << endl;
  return false;
}

// -- Rule 4: "738: Nothing is correct"
inline bool rule_4(int first, int second, int third)
{
  // 7,3,8 can't be anywhere
  bool no_digit_7 = (first != 7 && second != 7 && third != 7);
  bool no_digit_3 = (first != 3 && second != 3 && third != 3);
  bool no_digit_8 = (first != 8 && second != 8 && third != 8);

  if (no_digit_3 && no_digit_7 && no_digit_8)
  {
    cout << first << second << third << ": rule 4: match" << endl;
    return true;
  }

  cout << first << second << third << ": rule 4: fail" << endl;
  return false;
}

// -- Rule 5: "780: One number is correct but wrong placed"
inline bool rule_5(int first, int second, int third)
{
  bool first_correct_but_wrong_placed = ((first == 8 || first == 0) && (first != 7)) ? true : false;
  bool second_correct_but_wrong_placed = ((second == 7 || second == 0) && (second != 8)) ? true : false;
  bool third_correct_but_wrong_placed = ((third == 7 || third == 8) && (third != 0)) ? true : false;

  if  (
       (first_correct_but_wrong_placed == true  && second_correct_but_wrong_placed == false && third_correct_but_wrong_placed == false)
    || (first_correct_but_wrong_placed == false && second_correct_but_wrong_placed == true  && third_correct_but_wrong_placed == false)
    || (first_correct_but_wrong_placed == false && second_correct_but_wrong_placed == false && third_correct_but_wrong_placed == true )
  )
  {
    cout << first << second << third << ": rule 5: match" << endl;
    return true;
  }

  cout << first << second << third << ": rule 5: fail" << endl;
  return false;
}

// -- Test all 5 rules
inline bool test(int input)
{
  int first = input / 100;
  int second = (input / 10) % 10;
  int third = input % 10;

  if (rule_1(first, second, third))
    if (rule_2(first, second, third))
      if (rule_3(first, second, third))
        if (rule_4(first, second, third))
          if (rule_5(first, second, third))
            return true;

  return false;
}

// --
void find_triple_digit_combinations() {
  // Try all numbers 000,001,...999
  for (register int input = 0; input < 1000; input++) {
    cout << input << ": testing..." << endl;
    if (test(input)) {
      if (input < 100) {
        cout << '0';
      }
      cout << input;
      cout << ": found match !" << endl;
    }
  }
}

// --
int main() {
  find_triple_digit_combinations();
  return 0;
}

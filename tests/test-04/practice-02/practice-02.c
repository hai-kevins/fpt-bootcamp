#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

// === ONLY CODE WITHIN THE DESIGNATED AREA . DO NOT MODIFY OTHER PARTS ===

const char *p;
bool ok;

void skipSpace() {
    while (*p && isspace(*p)) p++;
}

double parseExpression();

double parseNumber() {
    skipSpace();

    if (!isdigit(*p)) {
        ok = false;
        return 0;
    }

    double val = 0;

    while (isdigit(*p)) {
        val = val * 10 + (*p - '0');
        p++;
    }

    if (*p == '.') {
        p++;

        if (!isdigit(*p)) {
            ok = false;
            return 0;
        }

        double base = 10;

        while (isdigit(*p)) {
            val += (*p - '0') / base;
            base *= 10;
            p++;
        }
    }

    return val;
}

double parseFactor() {
    skipSpace();

    if (*p == '(') {
        p++;
        double val = parseExpression();
        skipSpace();

        if (*p != ')') {
            ok = false;
            return 0;
        }

        p++;
        return val;
    }

    return parseNumber();
}

double parseTerm() {
    double val = parseFactor();

    while (ok) {
        skipSpace();

        if (p == '') {
            p++;
            val *= parseFactor();
        }
        else if (*p == '/') {
            p++;
            double x = parseFactor();

            if (!ok) return 0;

            if (fabs(x) < 1e-12) {
                ok = false;
                return 0;
            }

            val /= x;
        }
        else
            break;
    }

    return val;
}

double parseExpression() {
    double val = parseTerm();

    while (ok) {
        skipSpace();

        if (*p == '+') {
            p++;
            val += parseTerm();
        }
        else if (*p == '-') {
            p++;
            val -= parseTerm();
        }
        else
            break;
    }

    return val;
}

bool evaluateExpression(const char* expression, long long* result) {
    p = expression;
    ok = true;

    skipSpace();

    if (*p == '\0')
        return false;

    // Không cho phép dấu + hoặc - đơn ngôi
    if (*p == '+' || *p == '-')
        return false;

    double val = parseExpression();

    skipSpace();

    if (!ok || *p != '\0') {
        *result = 0;
        return false;
    }

    if (val >= 0)
        *result = (long long)(val + 0.5);
    else
        *result = -(long long)((-val) + 0.5);

    return true;
}
// === DO NOT MODIFY OTHER PARTS ===

int main() {
    char expression[1005];
    if (!fgets(expression, sizeof(expression), stdin)) return 0;
    expression[strcspn(expression, "\r\n")] = '\0';
    long long result = 0;
    bool ok = evaluateExpression(expression, &result);
    printf("\n__JUDGE_RESULT__\n");
    if (ok) printf("true %lld", result);
    else printf("false 0");
    return 0;
}
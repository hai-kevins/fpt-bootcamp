#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char *email;
    char *name;
    int parent;
    int rank;
} EmailEntry;

typedef struct
{
    int root;
    char *name;
    char **emails;
    int size;
    int capacity;
} AccountGroup;

static int findRoot(EmailEntry *entries, int index)
{
    if (entries[index].parent != index)
    {
        entries[index].parent = findRoot(entries, entries[index].parent);
    }

    return entries[index].parent;
}

static void unionEntries(EmailEntry *entries, int left, int right)
{
    int leftRoot = findRoot(entries, left);
    int rightRoot = findRoot(entries, right);

    if (leftRoot == rightRoot)
    {
        return;
    }

    if (entries[leftRoot].rank < entries[rightRoot].rank)
    {
        entries[leftRoot].parent = rightRoot;
    }
    else if (entries[leftRoot].rank > entries[rightRoot].rank)
    {
        entries[rightRoot].parent = leftRoot;
    }
    else
    {
        entries[rightRoot].parent = leftRoot;
        entries[leftRoot].rank++;
    }
}

static int findEmail(EmailEntry *entries, int entryCount, const char *email)
{
    for (int index = 0; index < entryCount; index++)
    {
        if (strcmp(entries[index].email, email) == 0)
        {
            return index;
        }
    }

    return -1;
}

static int compareStrings(const void *left, const void *right)
{
    const char *const *leftString = left;
    const char *const *rightString = right;

    return strcmp(*leftString, *rightString);
}

static void freeGroups(AccountGroup *groups, int groupCount)
{
    for (int index = 0; index < groupCount; index++)
    {
        free(groups[index].emails);
    }

    free(groups);
}

char ***accountsMerge(
    char ***accounts,
    int accountsSize,
    int *accountsColSize,
    int *returnSize,
    int **returnColumnSizes)
{
    int maximumEmails = 0;

    *returnSize = 0;
    *returnColumnSizes = NULL;

    for (int account = 0; account < accountsSize; account++)
    {
        maximumEmails += accountsColSize[account] - 1;
    }

    EmailEntry *entries = malloc((size_t)maximumEmails * sizeof(*entries));

    if ((entries == NULL) && (maximumEmails > 0))
    {
        return NULL;
    }

    int entryCount = 0;

    for (int account = 0; account < accountsSize; account++)
    {
        int firstIndex = -1;

        for (int column = 1; column < accountsColSize[account]; column++)
        {
            int index = findEmail(entries, entryCount, accounts[account][column]);

            if (index < 0)
            {
                index = entryCount++;
                entries[index].email = accounts[account][column];
                entries[index].name = accounts[account][0];
                entries[index].parent = index;
                entries[index].rank = 0;
            }

            if (firstIndex < 0)
            {
                firstIndex = index;
            }
            else
            {
                unionEntries(entries, firstIndex, index);
            }
        }
    }

    AccountGroup *groups = calloc((size_t)entryCount, sizeof(*groups));

    if ((groups == NULL) && (entryCount > 0))
    {
        free(entries);
        return NULL;
    }

    int groupCount = 0;

    for (int index = 0; index < entryCount; index++)
    {
        int root = findRoot(entries, index);
        int groupIndex = -1;

        for (int group = 0; group < groupCount; group++)
        {
            if (groups[group].root == root)
            {
                groupIndex = group;
                break;
            }
        }

        if (groupIndex < 0)
        {
            groupIndex = groupCount++;
            groups[groupIndex].root = root;
            groups[groupIndex].name = entries[root].name;
            groups[groupIndex].capacity = 4;
            groups[groupIndex].emails = malloc(
                (size_t)groups[groupIndex].capacity * sizeof(char *));

            if (groups[groupIndex].emails == NULL)
            {
                freeGroups(groups, groupCount);
                free(entries);
                return NULL;
            }
        }

        if (groups[groupIndex].size == groups[groupIndex].capacity)
        {
            int newCapacity = groups[groupIndex].capacity * 2;
            char **newEmails = realloc(
                groups[groupIndex].emails,
                (size_t)newCapacity * sizeof(char *));

            if (newEmails == NULL)
            {
                freeGroups(groups, groupCount);
                free(entries);
                return NULL;
            }

            groups[groupIndex].emails = newEmails;
            groups[groupIndex].capacity = newCapacity;
        }

        groups[groupIndex].emails[groups[groupIndex].size++] = entries[index].email;
    }

    char ***result = malloc((size_t)groupCount * sizeof(*result));
    int *columnSizes = malloc((size_t)groupCount * sizeof(*columnSizes));

    if (((result == NULL) || (columnSizes == NULL)) && (groupCount > 0))
    {
        free(result);
        free(columnSizes);
        freeGroups(groups, groupCount);
        free(entries);
        return NULL;
    }

    for (int group = 0; group < groupCount; group++)
    {
        qsort(
            groups[group].emails,
            (size_t)groups[group].size,
            sizeof(char *),
            compareStrings);

        columnSizes[group] = groups[group].size + 1;
        result[group] = malloc((size_t)columnSizes[group] * sizeof(char *));

        if (result[group] == NULL)
        {
            for (int previous = 0; previous < group; previous++)
            {
                free(result[previous]);
            }

            free(result);
            free(columnSizes);
            freeGroups(groups, groupCount);
            free(entries);
            return NULL;
        }

        result[group][0] = groups[group].name;

        for (int email = 0; email < groups[group].size; email++)
        {
            result[group][email + 1] = groups[group].emails[email];
        }
    }

    *returnSize = groupCount;
    *returnColumnSizes = columnSizes;

    freeGroups(groups, groupCount);
    free(entries);
    return result;
}

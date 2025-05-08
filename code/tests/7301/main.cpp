#include <bits/stdc++.h>
#define x first
#define y second
#define pb push_back
#define mp make_pair
#define all(x) (x).begin(), (x).end()
#define lowbit(x) ((x) & -(x))
#define SZ(x) (int)x.size()
#define endl '\n'
using namespace std;
using ll = long long;
using ld = long double;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using pdd = pair<ld, ld>;
mt19937 mrand(random_device{}());
int rnd(int x) { return mrand() % x; }
const int INF = 0x3f3f3f3f;
const int N = 5e4 + 10;
const double gap = 0.75;
#define ls tr[u].l
#define rs tr[u].r
struct point
{
    int num[5];
    int val;
    bool operator<(const point &p) const
    {
        for (int i = 0; i < 5; i++)
            if (num[i] >= p.num[i])
                return false;
        return true;
    }
    bool operator>=(const point &p) const
    {
        for (int i = 0; i < 5; i++)
            if (num[i] < p.num[i])
                return false;
        return true;
    }
    bool operator>(const point &p) const
    {
        for (int i = 0; i < 5; i++)
            if (num[i] <= p.num[i])
                return false;
        return true;
    }
    bool operator<=(const point &p) const
    {
        for (int i = 0; i < 5; i++)
            if (num[i] > p.num[i])
                return false;
        return true;
    }
} pos[N];
struct node
{
    int l, r;            // l是左儿子,r是右儿子
    point p, gapl, gapr; // 当前点和两个边界
    int sz;
    int ma;
} tr[N];
void pushup(int u)
{
    tr[u].ma = max({tr[u].p.val, tr[ls].ma, tr[rs].ma});
    tr[u].sz = tr[ls].sz + tr[rs].sz + 1;
    for (int i = 0; i < 5; i++)
    {
        tr[u].gapl.num[i] = min({tr[u].p.num[i], tr[ls].gapl.num[i], tr[rs].gapl.num[i]});
        tr[u].gapr.num[i] = max({tr[u].p.num[i], tr[ls].gapr.num[i], tr[rs].gapr.num[i]});
    }
}
bool in(node &u, point &gapl, point &gapr)
{ // 某矩阵是否在该矩阵内
    return u.gapl >= gapl && u.gapr <= gapr;
}
bool in(point &p, point &gapl, point &gapr)
{ // 某点是否在该矩阵中
    return p >= gapl && p <= gapr;
}
bool out(node &u, point &gapl, point &gapr)
{ // 某矩阵是否在该矩阵外
    for (int i = 0; i < 5; i++)
    {
        if (
            u.gapl.num[i] > gapr.num[i] || u.gapr.num[i] < gapl.num[i])
            return true;
    }
    return false;
}
void query(int u, point &tar, int &res)
{
    if (!u || res > tr[u].ma)
        return;
    for (int i = 0; i < 5; i++)
        if (tar.num[i] < tr[u].gapl.num[i])
            return;
    if (tar >= tr[u].gapr)
    {
        return res = max(res, tr[u].ma), void();
    }
    if (tar >= tr[u].p)
        res = max(res, tr[u].p.val);
    query(ls, tar, res);
    query(rs, tar, res);
}

int n, idx, root, buf[N], tot, compare; // compare为0~5比较a,b,c,d,e
// buf是可用的节点,tot是可用的节点数
bool cmp(const point &a, const point &b)
{
    return a.num[compare] < b.num[compare];
}
int getnode()
{
    if (!tot)
        return ++idx;
    return buf[tot--];
}
void getseq(int u, int cnt)
{
    if (ls)
        getseq(ls, cnt);
    buf[++tot] = u, pos[tr[ls].sz + 1 + cnt] = tr[u].p;
    if (rs)
        getseq(rs, cnt + tr[ls].sz + 1);
}
int rebuild(int l, int r, int k)
{
    if (l > r)
        return 0;
    int mid = l + r >> 1;
    compare = k;
    int u = getnode();
    nth_element(pos + l, pos + mid, pos + r + 1, cmp);
    tr[u].p = pos[mid];
    ls = rebuild(l, mid - 1, (k + 1) % 5), rs = rebuild(mid + 1, r, (k + 1) % 5);
    pushup(u);
    return u;
}
void maintain(int &u, int k)
{ // 均摊O(log n)
    if (tr[u].sz * gap < tr[ls].sz || tr[u].sz * gap < tr[rs].sz)
        getseq(u, 0), u = rebuild(1, tot, k);
}
void insert(int &u, const point &p, int k)
{
    if (!u)
    {
        u = getnode();
        tr[u].l = tr[u].r = 0;
        tr[u].p = p;
        pushup(u);
        return;
    }
    compare = k;
    if (cmp(p, tr[u].p))
        insert(ls, p, (k + 1) % 5);
    else
        insert(rs, p, (k + 1) % 5);
    pushup(u);
    maintain(u, k);
}
void init()
{
    tr[0].gapl = {INF, INF, INF, INF, INF};
    tr[0].gapr = {-INF, -INF, -INF, -INF, -INF};
}
void solve()
{
    root = idx = tot = 0;
    cin >> n;
    init();
    for (int i = 1; i <= n; i++)
    {
        point p;
        cin >> p.num[0] >> p.num[1] >> p.num[2] >> p.num[3] >> p.num[4] >> p.val;
        int res = 0;
        query(root, p, res);
        p.val += res;
        insert(root, p, 0);
        cout << p.val << endl;
    }
}
int main()
{
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
#ifdef LOCAL
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
    int t;
    cin >> t;
    while (t--)
    {
        solve();
    }
    return 0;
}
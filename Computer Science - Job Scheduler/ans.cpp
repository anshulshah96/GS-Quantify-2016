#include<bits/stdc++.h>
using namespace std;

// #define ll long long
#define inf 0x7fffffff
#define SCD(t) scanf("%d",&t)
#define SCLD(t) scanf("%ld",&t)
#define SCLLD(t) scanf("%lld",&t)
#define SCC(t) scanf("%c",&t)
#define SCS(t) scanf("%s",t)
#define SCF(t) scanf("%f",&t)
#define SCLF(t) scanf("%lf",&t)
#define pr pair<int,int>
#define mp(a,b) make_pair(a,b)
#define pb push_back
#define eb emplace_back
#define fr first
#define sc second
#define mset(arr,val) memset(arr,val,sizeof(arr));

typedef long long ll;

const int RTN = 3300;
const int MAX = 10000005;
ll cpu_tot, cpu_free; 
ll tm_asg = 0 ,tm_qt = 0, tm_qo = 0;

struct jd
{
	ll id;
	ll timestamp;
	string orig;
	string instr;
	ll importance;
	ll duration;
	jd(ll t,ll i,string o,string ins,ll imp,ll dur)
	{
		id = i;
		timestamp = t;
		orig = o;
		instr = ins;
		importance = imp;
		duration = dur;
	}
	jd()
	{}
};
struct snap
{
	ll ctime;
	// std::vector< jd > addj;
	// std::vector< string > remj;
	bool isaddj;
	jd* addj;
	string remj;
};

struct qcompare{
	bool operator()(jd* l, jd* r)
	{
		bool ans;
		if(l->importance != r->importance)
			ans = l->importance > r->importance;
		else if (l->timestamp != r->timestamp)
			ans = l->timestamp < r->timestamp;
		else 
			ans = l->duration < r->duration;
		return !ans;
	}
};

std::vector< snap > snap_vec;
priority_queue< jd*, vector<jd*>, qcompare > aspq;
priority_queue< ll, vector<ll>, greater<ll> > cpupq;

priority_queue< jd*, vector<jd*>, qcompare > qkpq;
map< string, priority_queue< jd*, vector<jd*>, qcompare > > qomp;

jd* mapjob[MAX];

void add_job(jd* job){
		snap sn;
		sn.ctime = job->timestamp;
		sn.addj = job;
		sn.isaddj = true;
		snap_vec.pb(sn);
}
void rem_job(string jorig,ll times){
		snap sn;
		sn.ctime = times;
		sn.remj = jorig;
		sn.isaddj = false;
		snap_vec.pb(sn);
}
void print_job(jd* job){
	cout<<"job "<<job->timestamp<<" "<<job->id<<" "<<job->orig<<" "<<job->instr<<" "<<job->importance<<" "<<job->duration<<endl;
}
void pre_time(ll times)
{
	if(cpupq.empty()) return;
	while(!cpupq.empty() && cpupq.top() <= times){
		cpu_free++;
		cpupq.pop();
	}
}
void see_snaps(ll times)
{
	cout<<"------------------";
	cout<<endl<<" FOR "<<times<<endl;
	cout<<"------------------"<<endl;
	for(int i = 0;i<snap_vec.size();i++)
	{
		snap sn= snap_vec[i];
		cout<<endl<<sn.ctime<<": "<<endl;
		if(sn.isaddj){
			print_job(sn.addj);
		}
		else
			cout<<sn.remj<<endl;
	}
}

bool check_no(string str)
{
	for(int i = 0;i<str.size();i++){
		if(str[i]>='0' && str[i]<='9'){}
		else return false;
	}

	return true;
}

int main(){
	freopen("input.txt","r",stdin);
	string temp;
	cin>>temp;
	cin>>cpu_tot;
	cpu_free = cpu_tot;
	int cnt = 0;

	while(cin)
	{
		string s;
		cin>>s;
		if(s == "") continue;
		else if(s[0]=='j')
		{
			cnt++;
			ll times,id,imp,dur;
			cin>>times>>id;
			string orig,instr;
			cin>>orig>>instr;
			cin>>imp>>dur;
			mapjob[cnt] = new jd(times,id,orig,instr,imp,dur);
			aspq.push(mapjob[cnt]);
			add_job(mapjob[cnt]);
			// see_snaps(times);
		}
		else if(s[0] == 'a')
		{
			ll times,k;
			cin>>times>>k;

			pre_time(times);   // do remaining tasks upto this timestamp

			// which top k have to be removed
			while(!aspq.empty() && k>0 && cpu_free>0)
			{
				k--;
				jd* job = aspq.top();
				aspq.pop();
				if(job->duration > 0){
					cpupq.push(job->duration+times);
					cpu_free--;
				}
				print_job(job);
				rem_job(job->orig,times);
			}

			// cout<<"BY ASSIGN "<<times<<endl;
			// see_snaps(times);
		}
		else if(s[0] == 'q')
		{
			ll times;
			cin>>times;
			string s2;
			cin>>s2;
			//nos. test
			if(check_no(s2))
			{
				stringstream ss;
				ss<<s2;
				ll k;
				ss>>k;
				int qkptr = 0;
				qkpq = priority_queue< jd*, vector<jd*>, qcompare >();
				while(qkptr<snap_vec.size() && snap_vec[qkptr].ctime<=times)
				{
					snap sn = snap_vec[qkptr];
					if(sn.isaddj)
					{
						qkpq.push(sn.addj);
					}
					else
					{
						qkpq.pop();
					}
					qkptr++;
				}

				queue< jd* > tempq;
				while(k>0 && !qkpq.empty())
				{
					k--;
					tempq.push(qkpq.top());
					qkpq.pop();
				}

				while(!tempq.empty())
				{
					jd* top = tempq.front();
					print_job(top);
					qkpq.push(top);
					tempq.pop();
				}
			}
			else{
				int qoptr = 0;
				qomp.clear();
				while(qoptr<snap_vec.size() && snap_vec[qoptr].ctime<=times)
				{
					snap sn = snap_vec[qoptr];
					if(sn.isaddj)
					{
						qomp[sn.addj->orig].push(sn.addj);
					}
					else
					{
						qomp[sn.remj].pop();
					}
					qoptr++;
				}

				priority_queue< jd*, vector<jd*>, qcompare > qtemp = qomp[s2];

				while(!qtemp.empty())
				{
					jd* top = qtemp.top();
					print_job(top);
					qtemp.pop();
				}
			}
		}
	}
	// see_snaps(11000LL);
}
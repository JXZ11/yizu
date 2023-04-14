void sub4_1()//专题4:Clock页面置换算法,默认低物理地址优先 
{
	int frm,nfp,ram[5],flg[5],prl[25],rsl[25],mor[5][25],t,a=0,b=0;//目前最多分配5个物理块,超出会出问题
	//frm:frame(分配物理块数) nfp:next frame pointer(替换指针)
	//ram(当前内存情况) flg:flag(当前访问位情况) t:temp(临时变量) a,b(分别记录PF和PR)
	//prl:page replacement list(页面置换情况)
	//rsl:reference string list(页面访问串)
	//mor:matrix of ram(RAM的页表变化情况)
	for(int i=0;i<5;i++) ram[i]=flg[i]=-1;
	for(int i=0;i<25;i++) prl[i]=rsl[i]=-1;
	for(int i=0;i<5;i++) for(int j=0;j<25;j++) mor[i][j]=-1;//初始化-1,最后只输出非负值即可 
	cout<<"输入所分配的物理块数(最高为5)"<<endl;
	cin>>frm;
	cout<<"输入NF指针初始指向,高地址输入1(正值),低地址输入0"<<endl;
	cin>>nfp;
	if(nfp) nfp=frm-1;
	cout<<"输入已装入内存中的页面号,空格间隔,最后换行"<<endl;//最后添加-1(负值) 
	for(int i=0;cin>>ram[i];i++) if(getchar()=='\n') break;//修改后不需要加-1了 
//	for(int i=0;cin>>t;i++)//t只是为了防止越界,因为开数组的时候卡的极限,不想整大了
//	{
//		if(t<0) break;//输入完毕直接跳出(空置内存块默认-1)
//		ram[i]=t;
//	}
	cout<<"输入内存中页面对应访问位,空格间隔,最后换行"<<endl;//最后添加-1(负值) 
	for(int i=0;cin>>flg[i];i++) if(getchar()=='\n') break;
//	for(int i=0;cin>>t;i++)
//	{
//		if(t<0) break;//输入完毕直接跳出(剩余访问位默认-1) 
//		else if(t) flg[i]=1;//标记访问位
//		else flg[i]=0;
//	}
	cout<<"依次输入页面访问串值,空格间隔,最后换行"<<endl;//最后添加-1(负值) 
	for(int i=0;cin>>rsl[i];i++) if(getchar()=='\n') break;
//	for(int i=0;cin>>t;i++)
//	{
//		if(t<0) {t=i;break;}//t此处记录页面访问串长度,虽然这个值没啥用了,但t变量也没啥用了 
//		rsl[i]=t;
//	}
	for(int i=0;rsl[i]>=0&&i<25;i++)//i此处表页面访问次数(页面访问串下标),i<25是为了防止越界(其实应该把数组开大点的,这太麻烦了) 
	{
		int j;
		for(j=0;j<frm;j++) if(ram[j]==rsl[i])//检查当前ram里是否有要找的页面rsl[i],如果有则 
		{
			flg[j]=1;//访问位置1 
			j=-1;//记录一下有
			break;
		}
		if(j<0) continue;//如果有要找的页面,则不发生缺页中断,直接找下一个访问页(与nfp无关)
		while(1)//后半部分语句均为缺页中断情况,循环查找内存
		{//nfp++%=frm为必带语句,但置换后需要break所以分开写了两次 
			if(flg[nfp]>0)
			{
				flg[nfp]=0;//若访问位为1,则置0
				nfp++;//nfp指针后移 
				nfp%=frm;//nfp指针超出所分配的物理块后会循环指向第一块
			}
			else//否则或者访问位为0进行页面置换,或者分配块为空直接装入 
			{
				a++;//页面中断次数PF增加 
				prl[i]=ram[nfp];//记录:prl的i位置记录被置换出来的页面号,即当前nfp指针指向的ram中的页(如果是空块就是-1赋值给-1,不碍事)
				ram[nfp]=rsl[i];//置换:指针当前所指页面被置换为当前访问页面rsl[i]
				flg[nfp]=1;//置换:新加入页面默认访问位置1; 
				for(j=0;j<frm;j++) mor[j][i]=ram[j];//记录:mor记录当前页面置换后的情况,此处j为ram遍历(0-frm),i为列号 
				nfp++;//nfp指针后移
				nfp%=frm;//nfp指针超出所分配的物理块后会循环指向第一块
				break;
			}
		}
	}
	cout<<endl<<endl<<" PR:|";
	for(int i=0;rsl[i]>=0&&i<25;i++)
	{
		if(prl[i]>=0) {cout<<prl[i]<<"|";b++;}//顺便记录PR 
		else cout<<" |"; 
	}
	cout<<endl<<endl<<" RS:|";
	for(int i=0;rsl[i]>=0&&i<25;i++) cout<<rsl[i]<<"|";
	cout<<endl<<endl<<"RAM:|";
	for(int j=0;j<frm;j++)
	{
		if(j) cout<<"    |";
		for(int i=0;rsl[i]>=0&&i<25;i++)
		{
			if(mor[j][i]>=0) cout<<mor[j][i]<<"|";
			else cout<<" |";
		}
		cout<<endl;
	}
	cout<<endl<<" PF ="<<a<<endl;
	cout<<endl<<" PR ="<<b<<endl;
	cout<<endl<<"Pages to be replaced"<<endl<<"list:";
	t=b;//记一下PR,方便输出列表 
	for(int i=0;rsl[i]>=0&&i<25;i++) if(prl[i]>=0)
	{
		if(t-b) cout<<",";//第一个值前不输出"," 
		t--;
		cout<<prl[i];
	}
	cout<<endl<<endl<<"Page P and access bit A"<<endl<<"list:";
	for(int i=0;i<frm;i++)//有的最后要求不一样 
	{
		if(i) cout<<",";
		cout<<ram[i]<<"-"<<flg[i];
	}
}
void sub4_2()//专题4:Optimal页面置换算法,默认低物理地址优先
{
	int frm,ram[5],flg[5],prl[25],rsl[25],mor[5][25],t,a=0,b=0;//目前最多分配5个物理块,超出会出问题
	//frm:frame(分配物理块数) ram(当前内存情况) flg:flag(标记用)
	//prl:page replacement list(页面置换情况)
	//rsl:reference string list(页面访问串)
	//mor:matrix of ram(RAM的页表变化情况)
	//t:temp(临时变量) a,b(分别记录PF和PR)
	for(int i=0;i<5;i++) ram[i]=flg[i]=-1;//flg没啥用,只把没用的ram空间标记个-1 
	for(int i=0;i<25;i++) prl[i]=rsl[i]=-1;
	for(int i=0;i<5;i++) for(int j=0;j<25;j++) mor[i][j]=-1;//初始化-1,最后只输出非负值即可 
	cout<<"输入所分配的物理块数(最高为5)"<<endl;
	cin>>frm;
	cout<<"输入已装入内存中的页面号,空格间隔,最后换行"<<endl;
	for(int i=0;cin>>ram[i];i++) if(getchar()=='\n') break;
	cout<<"依次输入页面访问串值,空格间隔,最后换行"<<endl;
	for(int i=0;cin>>rsl[i];i++) if(getchar()=='\n') break;
	for(int i=0;rsl[i]>=0&&i<25;i++)//i此处表页面访问次数(页面访问串下标),i<25是为了防止越界(其实应该把数组开大点的,这太麻烦了) 
	{
		int j; 
		for(j=0;j<frm;j++)//每次访问页面,先检查一下内存里有无此页或者空块
		{
			if(ram[j]==rsl[i]) break;//如果有此页面(即使有空块,所以先检查是否有此页)
			else if(ram[j]==-1)//无此页面但有空块
			{
				a++;//没有页面置换b但有缺页中断a  
				ram[j]=rsl[i];//空块应当填入此访问页面 
				for(t=0;t<frm;t++) mor[t][i]=ram[t];//记录:内存变化后情况(剩余空块依旧-1不影响)
				break;//填上之后记录完毕也不用继续查了 
			}
		}
		if(j-frm) continue;
		//如果j!=frm,意味着上方循环是强制跳出,有访问页面或者有空块进行处理,不必进入缺页中断步骤
		//否则一整轮检查完毕后到下方意味着已经没有空块(ram分配块已满)且产生了缺页中断,必定有页面置换
		for(j=0;j<frm;j++)//对于ram中已有的每个页面
		{
			flg[j]=25;//标记初始化,假设访问串后方都无此页面(最大访问顺序下标为24,25找不到) 
			for(t=i+1;rsl[t]>=0&&t<25;t++) if(ram[j]==rsl[t]) //向后(t=i+1)检查访问串,如果找到 
			{
				flg[j]=t;//就记录访问串中最近的此页面的下标t 
				break;//不再找了 
			}
		}//现在ram中每个页面的flg值都是访问串向后最近的此页面的下标
		for(j=t=0;j<frm;j++) if(flg[t]<flg[j]) t=j;//从前向后(默认低地址优先,从0)遍历flg,找到下标最大的页面(等下标时取前者,仅小于更新ram页号t)
		//此时t指向ram中再次调用时位置最远的(或者再也不用的)那些页面中序号最小物理地址最低的页,置换此页
		a++;//缺页中断PF 
		b++;//页面置换PR 
		prl[i]=ram[t];//记录:此次要访问的是串中下标i的页面,被替换出来的页面ram[t]记录在prl的下标i处
		ram[t]=rsl[i];//记录完毕后将ram此页更新为要访问的页面
		for(j=0;j<frm;j++) mor[j][i]=ram[j];//记录:内存变化后情况
	}
	cout<<endl<<endl<<" PR:|";
	for(int i=0;rsl[i]>=0&&i<25;i++)
	{
		if(prl[i]>=0) cout<<prl[i]<<"|";//b++可以顺便记录PR,但之前记了 
		else cout<<" |"; 
	}
	cout<<endl<<endl<<" RS:|";
	for(int i=0;rsl[i]>=0&&i<25;i++) cout<<rsl[i]<<"|";
	cout<<endl<<endl<<"RAM:|";
	for(int j=0;j<frm;j++)
	{
		if(j) cout<<"    |";
		for(int i=0;rsl[i]>=0&&i<25;i++)
		{
			if(mor[j][i]>=0) cout<<mor[j][i]<<"|";
			else cout<<" |";
		}
		cout<<endl;
	}
	cout<<endl<<" PF ="<<a<<endl;
	cout<<endl<<" PR ="<<b<<endl;
	cout<<endl<<"Pages to be replaced"<<endl<<"list:";
	t=b;//记一下PR,方便输出列表
	for(int i=0;rsl[i]>=0&&i<25;i++) if(prl[i]>=0)
	{
		if(t-b) cout<<",";//第一个值前不输出"," 
		t--;
		cout<<prl[i];
	}
}
void sub4_3()//专题4:LRU页面置换算法,默认低物理地址优先(Optimal改的)
{
	int frm,ram[5],flg[5],prl[25],rsl[25],mor[5][25],t,a=0,b=0;//目前最多分配5个物理块,超出会出问题
	//frm:frame(分配物理块数) ram(当前内存情况) flg:flag(标记用)
	//prl:page replacement list(页面置换情况)
	//rsl:reference string list(页面访问串)
	//mor:matrix of ram(RAM的页表变化情况)
	//t:temp(临时变量) a,b(分别记录PF和PR)
	for(int i=0;i<5;i++) ram[i]=flg[i]=-1;//flg没啥用,只把没用的ram空间标记个-1 
	for(int i=0;i<25;i++) prl[i]=rsl[i]=-1;
	for(int i=0;i<5;i++) for(int j=0;j<25;j++) mor[i][j]=-1;//初始化-1,最后只输出非负值即可 
	cout<<"输入所分配的物理块数(最高为5)"<<endl;
	cin>>frm;
	cout<<"依次输入已装入内存中的页面号,空格间隔,最后换行"<<endl;
	for(int i=0;cin>>ram[i];i++) if(getchar()=='\n') break;
	cout<<"依次输入页面访问串值,空格间隔,最后换行"<<endl;
	for(int i=0;cin>>rsl[i];i++) if(getchar()=='\n') break;
	for(int i=0;rsl[i]>=0&&i<25;i++)//i此处表页面访问次数(页面访问串下标),i<25是为了防止越界(其实应该把数组开大点的,这太麻烦了) 
	{
		int j; 
		for(j=0;j<frm;j++)//每次访问页面,先检查一下内存里有无此页或者空块
		{
			if(ram[j]==rsl[i]) break;//如果有此页面(即使有空块,所以先检查是否有此页)
			else if(ram[j]==-1)//无此页面但有空块
			{
				a++;//没有页面置换b但有缺页中断a  
				ram[j]=rsl[i];//空块应当填入此访问页面 
				for(t=0;t<frm;t++) mor[t][i]=ram[t];//记录:内存变化后情况(剩余空块依旧-1不影响)
				break;//填上之后记录完毕也不用继续查了 
			}
		}
		if(j-frm) continue;
		//如果j!=frm,意味着上方循环是强制跳出,有访问页面或者有空块进行处理,不必进入缺页中断步骤
		//否则一整轮检查完毕后到下方意味着已经没有空块(ram分配块已满)且产生了缺页中断,必定有页面置换
		for(j=0;j<frm;j++)//对于ram中已有的每个页面
		{
			flg[j]=-1;//标记初始化,假设访问串前方都无此页面(最小访问顺序下标为0,-1找不到) 
			for(t=i-1;t>=0&&rsl[t]>=0;t--) if(ram[j]==rsl[t]) //向前(t=i-1)检查访问串,如果找到 
			{
				flg[j]=t;//就记录访问串中最近的此页面的下标t 
				break;//不再找了 
			}
		}//现在ram中每个页面的flg值都是访问串向前最近的此页面的下标
		for(j=t=0;j<frm;j++) if(flg[t]>flg[j]) t=j;//从前向后(默认低地址优先,从0)遍历flg,找到下标最小的页面(等下标时取前者,仅大于更新ram页号t)
		//此时t指向ram中再次调用时位置最远的(或者从来没用过的)那些页面中序号最小物理地址最低的页,置换此页
		a++;//缺页中断PF 
		b++;//页面置换PR 
		prl[i]=ram[t];//记录:此次要访问的是串中下标i的页面,被替换出来的页面ram[t]记录在prl的下标i处
		ram[t]=rsl[i];//记录完毕后将ram此页更新为要访问的页面
		for(j=0;j<frm;j++) mor[j][i]=ram[j];//记录:内存变化后情况
	}
	cout<<endl<<endl<<" PR:|";
	for(int i=0;rsl[i]>=0&&i<25;i++)
	{
		if(prl[i]>=0) cout<<prl[i]<<"|";//b++可以顺便记录PR,但之前记了 
		else cout<<" |"; 
	}
	cout<<endl<<endl<<" RS:|";
	for(int i=0;rsl[i]>=0&&i<25;i++) cout<<rsl[i]<<"|";
	cout<<endl<<endl<<"RAM:|";
	for(int j=0;j<frm;j++)
	{
		if(j) cout<<"    |";
		for(int i=0;rsl[i]>=0&&i<25;i++)
		{
			if(mor[j][i]>=0) cout<<mor[j][i]<<"|";
			else cout<<" |";
		}
		cout<<endl;
	}
	cout<<endl<<" PF ="<<a<<endl;
	cout<<endl<<" PR ="<<b<<endl;
	cout<<endl<<"Pages to be replaced"<<endl<<"list:";
	t=b;//记一下PR,方便输出列表
	for(int i=0;rsl[i]>=0&&i<25;i++) if(prl[i]>=0)
	{
		if(t-b) cout<<",";//第一个值前不输出"," 
		t--;
		cout<<prl[i];
	}
}
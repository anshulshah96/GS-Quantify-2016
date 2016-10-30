function joint(mu_oil,sigma_oil, mu_fx, sigma_fx)

X1 = xlsread("Input_Data.xlsx", "Joint_Oil_Call");	# Loading the FX Data
X2 = xlsread("Input_Data.xlsx", "Joint_FX_Put");

B2 = transpose(X2(:,1));		# The range of strike price
Q1 = transpose(X2(:,2));		# The price field

B1 = transpose(X1(:,1));		# The range of strike price
Q2 = transpose(X1(:,2));		# The price field


minerr = 1e9;
for rho = -1:0.01:1

	# Writing the formula for Q1 as derived in the document.

	M = (B2-mu_fx)./(sqrt(2*sigma_fx^2));
	I = ((2*sigma_fx^2)/(sqrt(pi)))*(1/4)*(sqrt(pi)*(erf(M)+1) - 2*e.^(-M.*M).*M);
	T2 = (B2-mu_fx)*(sigma_fx^2).*(normpdf(B2, mu_fx, sigma_fx)) - I;
	T1 = (B2-mu_fx).*(normcdf(B2, mu_fx, sigma_fx)) + (sigma_fx^2)*(normpdf(B2, mu_fx, sigma_fx));
	Q1_pred = mu_oil*T1 + rho*(sigma_oil/sigma_fx)*T2;

	# Writing the formula for Q2 as derived in the document.

	M = (B2-mu_oil)./(sqrt(2*sigma_oil^2));
	I = I = ((2*sigma_oil^2)/(sqrt(pi)))*(1/4)*(sqrt(pi)*erfc(M) + 2*e.^(-M.*M).*M);
	T2 = (mu_oil-B1)*(sigma_oil^2).*(normpdf(B1, mu_oil, sigma_oil)) + I;
	T1 = (mu_oil-B1).*(1-normcdf(B1, mu_oil, sigma_oil)) + (sigma_oil^2)*(normpdf(B1, mu_oil, sigma_oil));
	Q2_pred = mu_fx*T1 + rho*(sigma_fx/sigma_oil)*T2;

	# Estimating the error

	e1 = (Q1_pred-Q1).^2;
	e2 = (Q2_pred-Q2).^2;
	error = sum(e1)+sum(e2);
	if(error<minerr)
		minerr = error;
		minrho = rho;
		min_Q1 = Q1_pred;
		min_Q2 = Q2_pred;
	endif
endfor

minrho = 0.5;

mu1 = mu_oil;
mu2 = mu_fx;
sigma1 = sigma_oil;
sigma2 = sigma_fx;
X = xlsread("Output.xlsx", "OilCall_FXPut");
B1 = transpose(X(:,1));
B2 = transpose(X(:,2));

L1 = 2*(B1-mu1).*(B2-mu2)/(sigma1*sigma2);
L2 = (B1-mu1).^2./(sigma1^2) + (B2-mu2).^2./(sigma2^2);
L2 = -L2;
L3 = 1/(2*pi*sigma1*sigma2);

C1 = -(normcdf(B2, mu2, sigma2)).*(1-normcdf(B1, mu1, sigma1));
C2 = ((B2-mu2).*(normcdf(B2, mu2, sigma2)) + (sigma2^2)*normpdf(B2, mu2, sigma2)).*((mu1-B1).*(1-normcdf(B1, mu1, sigma1)) + (sigma1^2)*normpdf(B1, mu1, sigma1));

# Writing the final equation as derived in the documentation.

x = minrho;
a = L2;
b = L1;
I = (e.^(a/2)).*((x*x)/2+1/4*b*(x^3)/3+1/24*(x^4)/4*(4*a+b.^2+4)+1/192*b*(x^5)/5.*(12*a+b.^2+36)+((x^6)/6*(48*a.^2+24*a.*(b.^2+12)+b.^4+120*b.^2+144))/1920+...
	(b*(x^7)/7.*(240*a.^2+40*a.*(b.^2+60)+b.^4+280*b.^2+3600))/23040+((x^8)/8*(960*a.^3+720*a.^2.*(b.^2+20)+60*a.*(b.^4+168*b.^2+720)+b.^6+540*b.^4+25200*b.^2+14400))/322560);
I = I*L3 + C1*x + C2;

I = round(I*10^6)./10^6;

I = transpose(I);

r = xlswrite("Output.xlsx", I, "OilCall_FXPut", "C2:C127");		# Writing to the excel file

if (r==0)
	sprintf("Error in writing output values!\n");
endif



endfunction
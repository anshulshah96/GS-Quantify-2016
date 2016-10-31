function [min_mu, min_sigma] = fx_call()

X = xlsread("Input_Data.xlsx", "FX Call Option Prices");	# Loading the FX Data

K = transpose(X(:,1));		# The range of strike price
X = transpose(X(:,2));		# The price values 

# The below loop runs from mu values ranging 0 to 100
# and standard deviation ranging from 1 to 50
# for reasons mentioned in the doc.

minerr = 1e9;
for mu = 0:100
	for sigma = 1:0.1:50
		Y = sigma*sigma*normpdf(K, mu, sigma) + (mu-K).*(1-normcdf(K, mu, sigma));
		err = (Y-X).*(Y-X);
		err = err./X;
		err = sum(err);
		if (err<minerr)
			minerr = err;
			min_mu = mu;
			min_sigma = sigma;
			minY = Y;
		endif
	endfor
endfor

# Filling the output file

K = xlsread("Output.xlsx", "FX Digital");
K = transpose(K(:, 1));

# Using the formula for D(K, T; X) (derivation in the documentation)
X_pred = 1-normcdf(K, min_mu, min_sigma);
X_pred = transpose(X_pred);

X_pred = round(X_pred*10^6)./10^6;

r = xlswrite("Output.xlsx", X_pred, "FX Digital", "B2:B87");		# Writing to the excel file

if (r==0)
	sprintf("Error in writing output values!\n");
endif

# Filling the output file

K = xlsread("Output.xlsx", "FX Exotic");
K = transpose(K(:, 1));

M = (K-min_mu)./(sqrt(2*min_sigma^2));

# Using the formula for P(K, T; X) (derivation in the documentation)
X_pred = (2*(min_sigma^2)/(sqrt(pi)))*(1/4)*(sqrt(pi)*erfc(M) + 2*M.*e.^(-M.*M)) + 2*(min_mu-K)*(min_sigma^2).*normpdf(K, min_mu, min_sigma) + (min_mu-K).*(min_mu-K).*(1-normcdf(K, min_mu, min_sigma));
X_pred = transpose(X_pred);

X_pred = round(X_pred*10^6)./10^6;

r = xlswrite("Output.xlsx", X_pred, "FX Exotic", "B2:B87");		# Writing to the excel file

if (r==0)
	sprintf("Error in writing output values!\n");
endif


endfunction
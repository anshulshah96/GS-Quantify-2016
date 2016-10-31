# This is the main file. Run this file to generate the ouptut file.

pkg load io;

# Running the oil_call file

sprintf("Running the oil_call function!\n");

[mu_oil, sigma_oil] = oil_call();

# Running the fx_call file

sprintf("Running the fx_call function!\n");

[mu_fx, sigma_fx] = fx_call();

# Running the joint file

sprintf("Running the joint function!\n");

joint(mu_oil, sigma_oil, mu_fx, sigma_fx);
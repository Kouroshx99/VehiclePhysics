#pragma once

struct FTransientTireState
{
	// Transient deflections
	float u = 0.0f; // Longitudinal deflection [m]
	float v_alpha = 0.0f; // Lateral deflection due to side slip [m]
	float v_gamma = 0.0f; // Lateral deflection due to camber [m]
    
	// Transient slip quantities
	float kappa_prime = 0.0f; // Transient longitudinal slip
	float alpha_prime = 0.0f; // Transient slip angle [rad]
	float gamma_prime = 0.0f; // Transient camber [rad]

	// The slips actually handed to the Magic Formula: the state above, less the
	// low-speed damping of 7.26. Held separately because 7.26 modifies only what the
	// force is evaluated at - feeding it back into the integrator would move where
	// the standstill spring settles, when the point is to damp the approach to it.
	float kappa_prime_damped = 0.0f;
	float alpha_prime_damped = 0.0f;
    
	// Previous values for derivative calculation
	float prev_u = 0.0f;
	float prev_v_alpha = 0.0f;
	float prev_v_gamma = 0.0f;

	float prev_sigma_kappa = 0.f;
	float prev_sigma_alpha = 0.f;
    
	// Reset the state
	void Reset()
	{
		u = 0.0f;
		v_alpha = 0.0f;
		v_gamma = 0.0f;
		kappa_prime = 0.0f;
		alpha_prime = 0.0f;
		gamma_prime = 0.0f;
		kappa_prime_damped = 0.0f;
		alpha_prime_damped = 0.0f;
		prev_u = 0.0f;
		prev_v_alpha = 0.0f;
		prev_v_gamma = 0.0f;
	}
};
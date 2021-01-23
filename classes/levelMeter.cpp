/* The code from this levelMeter classis used from MOD gain + meter
 *
 * Copyright (C) 2016,2017 Robin Gareus <robin@gareus.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "levelMeter.hpp"

LevelMeter::LevelMeter()
{
    rate = 48000;
    level = 0.0;
    pre(128);
}

LevelMeter::~LevelMeter()
{
}

void LevelMeter::setSampleRate(float rate)
{
    this->rate = rate;
}

float fast_fabsf (float val)
{
	union {float f; int i;} t;
	t.f = val;
	t.i &= 0x7fffffff;
	return t.f;
}

float fast_log2 (float val)
{
	union {float f; int i;} t;
	t.f = val;
	int* const    exp_ptr =  &t.i;
	int           x = *exp_ptr;
	const int     log_2 = ((x >> 23) & 255) - 128;
	x &= ~(255 << 23);
	x += 127 << 23;
	*exp_ptr = x;
	val = ((-1.0f / 3.f) * t.f + 2) * t.f - 2.0f / 3.f;
	return (val + log_2);
}

float fast_log10f (const float val)
{
  return fast_log2(val) / 3.312500f;
}


void LevelMeter::pre(uint32_t n_samples)
{
	if (spp != n_samples) {
		const float fall = 15.0f;
		const float tme  = (float)n_samples / rate;
		falloff    = powf(10.0f, -0.05f * fall * tme);
		spp = n_samples;
	}
}

void LevelMeter::post()
{
	const float l = meterLevel;
	float dbLvl = l > 1e-6f  ? 20.f * fast_log10f(l) : -120;
	if (dbLvl == -120 && this->dbLvl != -120) {
		levelOut = 0;
		this->dbLvl = dbLvl;
	}
	else if (fast_fabsf (dbLvl - this->dbLvl) > .1) {
		levelOut = l;
		this->dbLvl = dbLvl;
	}
}

float LevelMeter::process(float *input, uint32_t n_samples)
{
	float l = meterLevel + 1e-20f;

	l *= falloff;

	// branches inside loop
	for (uint32_t i = 0; i < n_samples; ++i) {
		const float a = fast_fabsf (input[i]);
		if (a > l) { l = a; }
	}

	if (!isfinite (l)) l = 0;
	meterLevel = l;

	post();

    return levelOut;
}

float LevelMeter::stereoProcess(float *inputL, float *inputR, uint32_t n_samples)
{
	float l = meterLevel + 1e-20f;

	l *= falloff;

	// branches inside loop
	for (uint32_t i = 0; i < n_samples; ++i) {
		const float a = fast_fabsf ((inputL[i] + inputR[i])/2.0);
		if (a > l) { l = a; }
	}

	if (!isfinite (l)) l = 0;
	meterLevel = l;

	post();

    return levelOut;
}

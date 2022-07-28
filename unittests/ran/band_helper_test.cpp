/*
 *
 * Copyright 2013-2022 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#include "srsgnb/ran/band_helper.h"
#include "srsgnb/ran/channel_bandwidth.h"
#include "srsgnb/ran/ssb_mapping.h"
#include "srsgnb/ran/subcarrier_spacing.h"
#include "srsgnb/support/test_utils.h"

using namespace srsgnb;

void test_get_band_from_arfcn()
{
  TESTASSERT(band_helper::get_band_from_dl_arfcn(530000U) < UINT16_MAX);
  TESTASSERT_EQ(UINT16_MAX, band_helper::get_band_from_dl_arfcn(5300000U));
  TESTASSERT_EQ(1, band_helper::get_band_from_dl_arfcn(423000U));
  TESTASSERT_EQ(3, band_helper::get_band_from_dl_arfcn(365000U));
  TESTASSERT_EQ(5, band_helper::get_band_from_dl_arfcn(175000U));
  TESTASSERT_EQ(7, band_helper::get_band_from_dl_arfcn(530000U));
  TESTASSERT_EQ(25, band_helper::get_band_from_dl_arfcn(399000U));
  TESTASSERT_EQ(34, band_helper::get_band_from_dl_arfcn(404000U));
  TESTASSERT_EQ(38, band_helper::get_band_from_dl_arfcn(515000U));
  TESTASSERT_EQ(41, band_helper::get_band_from_dl_arfcn(499900U));
  TESTASSERT_EQ(41, band_helper::get_band_from_dl_arfcn(504000U));
  TESTASSERT_EQ(50, band_helper::get_band_from_dl_arfcn(286500U));
  TESTASSERT_EQ(51, band_helper::get_band_from_dl_arfcn(285500U));
  TESTASSERT_EQ(70, band_helper::get_band_from_dl_arfcn(400000U));
  TESTASSERT_EQ(77, band_helper::get_band_from_dl_arfcn(630500U));
}

void test_arfcn_freq_conversion()
{
  TESTASSERT_EQ(3489.42e6, band_helper::nr_arfcn_to_freq(632628));
  TESTASSERT_EQ(3508.92e6, band_helper::nr_arfcn_to_freq(633928)); // default refPointA
  TESTASSERT_EQ(3513.6e6, band_helper::nr_arfcn_to_freq(634240));  // default ARFCN with freq divisible by 11.52 MHz
  TESTASSERT_EQ(632628, band_helper::freq_to_nr_arfcn(3489.42e6));
  TESTASSERT_EQ(633928, band_helper::freq_to_nr_arfcn(3508.92e6));
  TESTASSERT_EQ(634240, band_helper::freq_to_nr_arfcn(3513.6e6));
  // n28 n67
  TESTASSERT_EQ(703.0e6, band_helper::nr_arfcn_to_freq(140600));
  TESTASSERT_EQ(729.0e6, band_helper::nr_arfcn_to_freq(145800));
  TESTASSERT_EQ(768.0e6, band_helper::nr_arfcn_to_freq(153600));
  TESTASSERT_EQ(788.0e6, band_helper::nr_arfcn_to_freq(157600));
  TESTASSERT_EQ(140600, band_helper::freq_to_nr_arfcn(703.0e6));
  TESTASSERT_EQ(145800, band_helper::freq_to_nr_arfcn(729.0e6));
  TESTASSERT_EQ(153600, band_helper::freq_to_nr_arfcn(768.0e6));
  TESTASSERT_EQ(157600, band_helper::freq_to_nr_arfcn(788.0e6));
  // n20
  TESTASSERT(band_helper::nr_arfcn_to_freq(158200) == 791.0e6);
  TESTASSERT(band_helper::nr_arfcn_to_freq(160200) == 801.0e6);
  TESTASSERT(band_helper::nr_arfcn_to_freq(162200) == 811.0e6);
  TESTASSERT(band_helper::nr_arfcn_to_freq(168400) == 842.0e6);
  // n32 n75
  TESTASSERT(band_helper::nr_arfcn_to_freq(290400) == 1452.0e6);
  TESTASSERT(band_helper::nr_arfcn_to_freq(294400) == 1472.0e6);

  // n1
  TESTASSERT(band_helper::nr_arfcn_to_freq(384000) == 1920.0e6);
  TESTASSERT(band_helper::nr_arfcn_to_freq(388030) == 1940.15e6);
  TESTASSERT(band_helper::nr_arfcn_to_freq(391830) == 1959.15e6);
  TESTASSERT(band_helper::nr_arfcn_to_freq(434000) == 2170.0e6);
  // n3
  TESTASSERT(band_helper::nr_arfcn_to_freq(342000) == 1710.0e6);
  TESTASSERT(band_helper::nr_arfcn_to_freq(348000) == 1740.0e6);
  TESTASSERT(band_helper::nr_arfcn_to_freq(361000) == 1805.0e6);
  TESTASSERT_EQ(1842.5e6, band_helper::nr_arfcn_to_freq(368500));
  TESTASSERT(band_helper::nr_arfcn_to_freq(376000) == 1880.0e6);
  TESTASSERT(band_helper::get_abs_freq_point_a_arfcn(52, 368500) == 367564);
  // n5
  TESTASSERT(band_helper::nr_arfcn_to_freq(176300) == 881.5e6);
  TESTASSERT(band_helper::freq_to_nr_arfcn(881.5e6) == 176300);
  TESTASSERT(band_helper::get_ul_arfcn_from_dl_arfcn(176300) == 167300);
  TESTASSERT(band_helper::nr_arfcn_to_freq(167300) == 836.5e6);
  TESTASSERT(band_helper::get_abs_freq_point_a_arfcn(52, 176300) == 175364);
  // n7 n38
  TESTASSERT(band_helper::nr_arfcn_to_freq(500000) == 2500.0e6);
  TESTASSERT(band_helper::nr_arfcn_to_freq(508000) == 2540.0e6);
  TESTASSERT(band_helper::nr_arfcn_to_freq(522000) == 2610.0e6);
  TESTASSERT(band_helper::nr_arfcn_to_freq(538000) == 2690.0e6);
  TESTASSERT(band_helper::get_abs_freq_point_a_arfcn(52, 529920) == 528984);

  // n78
  TESTASSERT(band_helper::nr_arfcn_to_freq(634240) == 3513.6e6);
  TESTASSERT(band_helper::get_abs_freq_point_a_arfcn(52, 634240) == 633928);

  const uint32_t max_valid_nr_arfcn = 3279165;

  // Max ARFCN is 3279165 at almost 10 GHz
  TESTASSERT(band_helper::nr_arfcn_to_freq(max_valid_nr_arfcn) == 99.99996e9);

  // Invalid ARFCN
  TESTASSERT(band_helper::nr_arfcn_to_freq(max_valid_nr_arfcn + 1) == 0.0);
}

void test_center_freq_conversion()
{
  TESTASSERT(band_helper::get_center_freq_from_abs_freq_point_a(52, 175364) == 881.5e6);
  TESTASSERT(band_helper::get_center_freq_from_abs_freq_point_a(52, 166364) == 836.5e6);
}

void test_is_paired_spectrum()
{
  TESTASSERT_EQ(true, band_helper::is_paired_spectrum(1));
  TESTASSERT_EQ(true, band_helper::is_paired_spectrum(3));
  TESTASSERT_EQ(true, band_helper::is_paired_spectrum(7));
  TESTASSERT_EQ(true, band_helper::is_paired_spectrum(25));
  TESTASSERT_EQ(true, band_helper::is_paired_spectrum(28));
  TESTASSERT_EQ(false, band_helper::is_paired_spectrum(34));
  TESTASSERT_EQ(false, band_helper::is_paired_spectrum(40));
  TESTASSERT_EQ(false, band_helper::is_paired_spectrum(51));
  TESTASSERT_EQ(true, band_helper::is_paired_spectrum(66));
  TESTASSERT_EQ(true, band_helper::is_paired_spectrum(74));
  TESTASSERT_EQ(false, band_helper::is_paired_spectrum(75));
  TESTASSERT_EQ(false, band_helper::is_paired_spectrum(76));
  TESTASSERT_EQ(false, band_helper::is_paired_spectrum(77));
  TESTASSERT_EQ(false, band_helper::is_paired_spectrum(79));
  TESTASSERT_EQ(false, band_helper::is_paired_spectrum(81));
  TESTASSERT_EQ(false, band_helper::is_paired_spectrum(84));
}

void test_ssb_pattern()
{
  TESTASSERT(band_helper::get_ssb_pattern(1, subcarrier_spacing::kHz15) < ssb_pattern_case::invalid);
  TESTASSERT_EQ(ssb_pattern_case::A, band_helper::get_ssb_pattern(1, subcarrier_spacing::kHz15));
  TESTASSERT_EQ(ssb_pattern_case::A, band_helper::get_ssb_pattern(3, subcarrier_spacing::kHz15));
  TESTASSERT_EQ(ssb_pattern_case::invalid, band_helper::get_ssb_pattern(3, subcarrier_spacing::kHz30));
  TESTASSERT_EQ(ssb_pattern_case::B, band_helper::get_ssb_pattern(5, subcarrier_spacing::kHz30));
  TESTASSERT_EQ(ssb_pattern_case::B, band_helper::get_ssb_pattern(5, subcarrier_spacing::kHz30));
  TESTASSERT_EQ(ssb_pattern_case::A, band_helper::get_ssb_pattern(25, subcarrier_spacing::kHz15));
  TESTASSERT_EQ(ssb_pattern_case::A, band_helper::get_ssb_pattern(34, subcarrier_spacing::kHz15));
  TESTASSERT_EQ(ssb_pattern_case::C, band_helper::get_ssb_pattern(34, subcarrier_spacing::kHz30));
  TESTASSERT_EQ(ssb_pattern_case::A, band_helper::get_ssb_pattern(38, subcarrier_spacing::kHz15));
  TESTASSERT_EQ(ssb_pattern_case::C, band_helper::get_ssb_pattern(38, subcarrier_spacing::kHz30));
  TESTASSERT_EQ(ssb_pattern_case::A, band_helper::get_ssb_pattern(39, subcarrier_spacing::kHz15));
  TESTASSERT_EQ(ssb_pattern_case::C, band_helper::get_ssb_pattern(39, subcarrier_spacing::kHz30));
  TESTASSERT_EQ(ssb_pattern_case::C, band_helper::get_ssb_pattern(40, subcarrier_spacing::kHz30));
  TESTASSERT_EQ(ssb_pattern_case::A, band_helper::get_ssb_pattern(41, subcarrier_spacing::kHz15));
  TESTASSERT_EQ(ssb_pattern_case::C, band_helper::get_ssb_pattern(41, subcarrier_spacing::kHz30));
  TESTASSERT_EQ(ssb_pattern_case::C, band_helper::get_ssb_pattern(50, subcarrier_spacing::kHz30));
  TESTASSERT_EQ(ssb_pattern_case::A, band_helper::get_ssb_pattern(51, subcarrier_spacing::kHz15));
  TESTASSERT_EQ(ssb_pattern_case::A, band_helper::get_ssb_pattern(66, subcarrier_spacing::kHz15));
  TESTASSERT_EQ(ssb_pattern_case::B, band_helper::get_ssb_pattern(66, subcarrier_spacing::kHz30));
  TESTASSERT_EQ(ssb_pattern_case::A, band_helper::get_ssb_pattern(70, subcarrier_spacing::kHz15));
  TESTASSERT_EQ(ssb_pattern_case::C, band_helper::get_ssb_pattern(78, subcarrier_spacing::kHz30));
}

void test_get_point_a_from_f_req()
{
  // Band n1, BWs are {5MHz, 10MHz, 15MHz, 20MHz}.
  TESTASSERT_EQ(2122150000.0,
                band_helper::get_abs_freq_point_a_from_f_ref(2124400000.0, 25, subcarrier_spacing::kHz15));
  TESTASSERT_EQ(2105420000.0,
                band_helper::get_abs_freq_point_a_from_f_ref(2110100000.0, 52, subcarrier_spacing::kHz15));
  TESTASSERT_EQ(2108090000.0,
                band_helper::get_abs_freq_point_a_from_f_ref(2115200000.0, 79, subcarrier_spacing::kHz15));
  TESTASSERT_EQ(2150860000.0,
                band_helper::get_abs_freq_point_a_from_f_ref(2160400000.0, 106, subcarrier_spacing::kHz15));

  // Band n3, BWs are {10MHz, 15MHz, 20MHz, 25MHz, 30MHz}.
  TESTASSERT_EQ(1821380000.0,
                band_helper::get_abs_freq_point_a_from_f_ref(1825700000.0, 24, subcarrier_spacing::kHz30));
  TESTASSERT_EQ(1844460000.0,
                band_helper::get_abs_freq_point_a_from_f_ref(1851300000.0, 38, subcarrier_spacing::kHz30));
  TESTASSERT_EQ(1860220000.0,
                band_helper::get_abs_freq_point_a_from_f_ref(1869400000.0, 51, subcarrier_spacing::kHz30));
  TESTASSERT_EQ(1837800000.0,
                band_helper::get_abs_freq_point_a_from_f_ref(1849500000.0, 65, subcarrier_spacing::kHz30));
  TESTASSERT_EQ(1837800000.0,
                band_helper::get_abs_freq_point_a_from_f_ref(1849500000.0, 65, subcarrier_spacing::kHz30));
  TESTASSERT_EQ(1842060000.0,
                band_helper::get_abs_freq_point_a_from_f_ref(1856100000.0, 78, subcarrier_spacing::kHz30));

  // Band n41, BWs are {40MHz, 50MHz, 60MHz, 70MHz, 80MHz, 90MHz, 100MHz}.
  TESTASSERT_EQ(2671620000.0,
                band_helper::get_abs_freq_point_a_from_f_ref(2689980000.0, 51, subcarrier_spacing::kHz60));
  TESTASSERT_EQ(2666580000.0,
                band_helper::get_abs_freq_point_a_from_f_ref(2689980000.0, 65, subcarrier_spacing::kHz60));
  TESTASSERT_EQ(2661540000.0,
                band_helper::get_abs_freq_point_a_from_f_ref(2689980000.0, 79, subcarrier_spacing::kHz60));
  TESTASSERT_EQ(2656500000.0,
                band_helper::get_abs_freq_point_a_from_f_ref(2689980000.0, 93, subcarrier_spacing::kHz60));
  TESTASSERT_EQ(2651460000.0,
                band_helper::get_abs_freq_point_a_from_f_ref(2689980000.0, 107, subcarrier_spacing::kHz60));
  TESTASSERT_EQ(2646420000.0,
                band_helper::get_abs_freq_point_a_from_f_ref(2689980000.0, 121, subcarrier_spacing::kHz60));
  TESTASSERT_EQ(2641380000.0,
                band_helper::get_abs_freq_point_a_from_f_ref(2689980000.0, 135, subcarrier_spacing::kHz60));
}

void test_get_from_f_req_point_a()
{
  // Band n1, BWs are {5MHz, 10MHz, 15MHz, 20MHz}.
  TESTASSERT_EQ(2124400000.0,
                band_helper::get_f_ref_from_abs_freq_point_a(2122150000.0, 25, subcarrier_spacing::kHz15));
  TESTASSERT_EQ(2110100000.0,
                band_helper::get_f_ref_from_abs_freq_point_a(2105420000.0, 52, subcarrier_spacing::kHz15));
  TESTASSERT_EQ(2115200000.0,
                band_helper::get_f_ref_from_abs_freq_point_a(2108090000.0, 79, subcarrier_spacing::kHz15));
  TESTASSERT_EQ(2160400000.0,
                band_helper::get_f_ref_from_abs_freq_point_a(2150860000.0, 106, subcarrier_spacing::kHz15));

  // Band n3, BWs are {10MHz, 15MHz, 20MHz, 25MHz, 30MHz}.
  TESTASSERT_EQ(1825700000.0,
                band_helper::get_f_ref_from_abs_freq_point_a(1821380000.0, 24, subcarrier_spacing::kHz30));
  TESTASSERT_EQ(1851300000.0,
                band_helper::get_f_ref_from_abs_freq_point_a(1844460000.0, 38, subcarrier_spacing::kHz30));
  TESTASSERT_EQ(1869400000.0,
                band_helper::get_f_ref_from_abs_freq_point_a(1860220000.0, 51, subcarrier_spacing::kHz30));
  TESTASSERT_EQ(1849500000.0,
                band_helper::get_f_ref_from_abs_freq_point_a(1837800000.0, 65, subcarrier_spacing::kHz30));
  TESTASSERT_EQ(1849500000.0,
                band_helper::get_f_ref_from_abs_freq_point_a(1837800000.0, 65, subcarrier_spacing::kHz30));
  TESTASSERT_EQ(1856100000.0,
                band_helper::get_f_ref_from_abs_freq_point_a(1842060000.0, 78, subcarrier_spacing::kHz30));

  // Band n41, BWs are {40MHz, 50MHz, 60MHz, 70MHz, 80MHz, 90MHz, 100MHz}.
  TESTASSERT_EQ(2689980000.0,
                band_helper::get_f_ref_from_abs_freq_point_a(2671620000.0, 51, subcarrier_spacing::kHz60));
  TESTASSERT_EQ(2689980000.0,
                band_helper::get_f_ref_from_abs_freq_point_a(2666580000.0, 65, subcarrier_spacing::kHz60));
  TESTASSERT_EQ(2689980000.0,
                band_helper::get_f_ref_from_abs_freq_point_a(2661540000.0, 79, subcarrier_spacing::kHz60));
  TESTASSERT_EQ(2689980000.0,
                band_helper::get_f_ref_from_abs_freq_point_a(2656500000.0, 93, subcarrier_spacing::kHz60));
  TESTASSERT_EQ(2689980000.0,
                band_helper::get_f_ref_from_abs_freq_point_a(2651460000.0, 107, subcarrier_spacing::kHz60));
  TESTASSERT_EQ(2689980000.0,
                band_helper::get_f_ref_from_abs_freq_point_a(2646420000.0, 121, subcarrier_spacing::kHz60));
  TESTASSERT_EQ(2689980000.0,
                band_helper::get_f_ref_from_abs_freq_point_a(2641380000.0, 135, subcarrier_spacing::kHz60));
}

void test_get_n_rbs_from_bw()
{
  TESTASSERT_EQ(
      25, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz5, subcarrier_spacing::kHz15, frequency_range::FR1));
  TESTASSERT_EQ(
      52, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz10, subcarrier_spacing::kHz15, frequency_range::FR1));
  TESTASSERT_EQ(
      79, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz15, subcarrier_spacing::kHz15, frequency_range::FR1));
  TESTASSERT_EQ(
      106, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz20, subcarrier_spacing::kHz15, frequency_range::FR1));
  TESTASSERT_EQ(
      133, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz25, subcarrier_spacing::kHz15, frequency_range::FR1));
  TESTASSERT_EQ(
      160, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz30, subcarrier_spacing::kHz15, frequency_range::FR1));
  TESTASSERT_EQ(
      216, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz40, subcarrier_spacing::kHz15, frequency_range::FR1));
  TESTASSERT_EQ(
      270, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz50, subcarrier_spacing::kHz15, frequency_range::FR1));
  TESTASSERT_EQ(
      0, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz60, subcarrier_spacing::kHz15, frequency_range::FR1));

  TESTASSERT_EQ(
      11, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz5, subcarrier_spacing::kHz30, frequency_range::FR1));
  TESTASSERT_EQ(
      24, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz10, subcarrier_spacing::kHz30, frequency_range::FR1));
  TESTASSERT_EQ(
      38, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz15, subcarrier_spacing::kHz30, frequency_range::FR1));
  TESTASSERT_EQ(
      51, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz20, subcarrier_spacing::kHz30, frequency_range::FR1));
  TESTASSERT_EQ(
      65, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz25, subcarrier_spacing::kHz30, frequency_range::FR1));
  TESTASSERT_EQ(
      78, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz30, subcarrier_spacing::kHz30, frequency_range::FR1));
  TESTASSERT_EQ(
      133, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz50, subcarrier_spacing::kHz30, frequency_range::FR1));
  TESTASSERT_EQ(
      162, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz60, subcarrier_spacing::kHz30, frequency_range::FR1));
  TESTASSERT_EQ(
      189, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz70, subcarrier_spacing::kHz30, frequency_range::FR1));
  TESTASSERT_EQ(
      217, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz80, subcarrier_spacing::kHz30, frequency_range::FR1));
  TESTASSERT_EQ(
      245, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz90, subcarrier_spacing::kHz30, frequency_range::FR1));
  TESTASSERT_EQ(
      273, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz100, subcarrier_spacing::kHz30, frequency_range::FR1));

  TESTASSERT_EQ(
      0, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz5, subcarrier_spacing::kHz60, frequency_range::FR1));
  TESTASSERT_EQ(
      11, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz10, subcarrier_spacing::kHz60, frequency_range::FR1));
  TESTASSERT_EQ(
      18, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz15, subcarrier_spacing::kHz60, frequency_range::FR1));
  TESTASSERT_EQ(
      24, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz20, subcarrier_spacing::kHz60, frequency_range::FR1));
  TESTASSERT_EQ(
      31, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz25, subcarrier_spacing::kHz60, frequency_range::FR1));
  TESTASSERT_EQ(
      38, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz30, subcarrier_spacing::kHz60, frequency_range::FR1));
  TESTASSERT_EQ(
      51, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz40, subcarrier_spacing::kHz60, frequency_range::FR1));
  TESTASSERT_EQ(
      65, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz50, subcarrier_spacing::kHz60, frequency_range::FR1));
  TESTASSERT_EQ(
      79, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz60, subcarrier_spacing::kHz60, frequency_range::FR1));
  TESTASSERT_EQ(
      93, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz70, subcarrier_spacing::kHz60, frequency_range::FR1));
  TESTASSERT_EQ(
      107, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz80, subcarrier_spacing::kHz60, frequency_range::FR1));
  TESTASSERT_EQ(
      121, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz90, subcarrier_spacing::kHz60, frequency_range::FR1));
  TESTASSERT_EQ(
      135, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz100, subcarrier_spacing::kHz60, frequency_range::FR1));

  TESTASSERT_EQ(
      0, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz100, subcarrier_spacing::kHz60, frequency_range::FR2));
  TESTASSERT_EQ(
      0, band_helper::get_n_rbs_from_bw(bs_channel_bw_fr1::MHz100, subcarrier_spacing::kHz120, frequency_range::FR1));
}

int main()
{
  test_get_band_from_arfcn();
  test_arfcn_freq_conversion();
  test_center_freq_conversion();
  test_is_paired_spectrum();
  test_ssb_pattern();
  test_get_point_a_from_f_req();
  test_get_from_f_req_point_a();
  test_get_n_rbs_from_bw();
}

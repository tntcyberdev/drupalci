<?php
/**
 * @file
 */

/**
 * Admin config form.
 */
function admin_example_form_config($form, &$form_state) {
  return system_settings_form($form);
}

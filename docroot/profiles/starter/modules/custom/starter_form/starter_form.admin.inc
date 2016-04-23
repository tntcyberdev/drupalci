<?php
/**
 * @file
 */

/**
 * Admin config form.
 */
function admin_example_form_config($form, &$form_state) {
  $form['sf_allow'] = array(
    '#type' => 'checkboxes',
    '#options' => array(
      'select' => t('Select'),
      'checkbox' => t('Checkbox'),
      'file' => t('File manage'),
      'text' => t('Text field'),
      'btn' => t('Button'),
    ),
  );

  return system_settings_form($form);
}

<?php

/**
 * @file
 * Contains FacetapiSelectDropdowns class.
 */

/**
 * Widget that renders facets as dropdowns.
 */
class FacetapiDateSelect extends FacetapiWidgetLinks {

  private $urls_cache = array();

  /**
   * {@inheritdoc}
   */
  public function execute() {
    static $count = 0;
    $count++;

    $element = &$this->build[$this->facet['field alias']];
    $settings = $this->settings;
    dpm($element);
    $variables = $this->buildItems($element);

    if (!empty($variables['options'])) {
      $reset_option_label = !empty($settings->settings['reset_option_label']) ? filter_xss_admin($settings->settings['reset_option_label']) : t('--Reset--');

      if (!empty($variables['facet_reset_url'])) {
        $variables['options'] = array_merge(
          array($variables['facet_reset_url'] => $reset_option_label),
          $variables['options']
        );
      }
      else {
        $default_option_label = !empty($settings->settings['default_option_label']) ? filter_xss_admin($settings->settings['default_option_label']) : t('--Choose--');
        $variables['default_option_label'] = theme('facetapi_select_select_option', array(
          'facet_text' => $default_option_label,
          'show_count' => FALSE
        ));
      }
    }

    $element = array(
      '#markup' => 'render Year - Month for filter',
    );
  }

  /**
   * Determine if the Facet is active.
   *
   * @return boolean
   *   Returns TRUE if the Facet is active.
   */
  public function isActive() {
   return (boolean) count($this->build['#adapter']->getActiveItems($this->facet->getFacet()));
  }

  /**
   * Build the options array for the provided facetapi element.
   *
   * @param array $element
   *   The facetapi element as provided by execute().
   *
   * @return array
   *   An array that contains the options and active items arrays.
   */
  protected function buildItems($element, $depth = 0) {
    $settings = $this->settings;
    $variables = array('options' => array());
    $facet_settings = $this->facet->getSettings();

    $url = NULL;
    foreach ($element as $value => $item) {
      $url = $this->getUrl($item);
      $item['#markup'] = str_repeat('-', $depth) . ' ' . $item['#markup'];
    }

    // When there is only one option and it is active with children let's add
    // the children as options.
    if ($item['#active'] && count($variables['options']) === 1 && !empty($item['#item_children'])) {
      $this->appendChildren($variables, $item, $depth);
    }

    return $variables;
  }

  /**
   * Append children to the active items and options for the facet.
   *
   * @param array $variables
   *   The variables array as provided by FacetapiSlectDropdowns::getItems().
   * @param array $item
   *   The facet item to use for retrieving the children.
   * @param int $depth
   *   The current depth of the childen.
   * @param array $is_active
   *   Dictates if the current item is active.
   *
   * @return void
   */
  protected function appendChildren(array &$variables, array $item, $depth = 0) {
    $tmpItems = $this->buildItems($item['#item_children'], ++$depth);

    if (!empty($tmpItems['options'])) {
      $variables['options'] += $tmpItems['options'];
    }
  }


  /**
   * Retrieve the url for the specified facet item.
   *
   * @param array $item
   *   The facet item to use for retrieving the url.
   * @param boolean $append_query (optional)
   *   Determines if the query should be appended to the return url.
   */
  protected function getUrl(array $item, $append_query = TRUE) {
    if (!isset($this->urls_cache[$item['#path']])) {
      $path = !empty($this->settings->settings['submit_page']) ? $this->settings->settings['submit_page'] : $item['#path'];
      $path = strpos($item['#path'], $path) === 0 ? $item['#path'] : $path;
      $this->urls_cache[$item['#path']] = $path;
    }

    $query = ($append_query) ? $item['#query'] : array();
    return url($this->urls_cache[$item['#path']], array('query' => $query));
  }

  /**
   * {@inheritdoc}
   */
  public function settingsForm(&$form, &$form_state) {
    parent::settingsForm($form, $form_state);

    $states = array(
      'visible' => array(
        'select[name="widget"]' => array('value' => $this->id),
      ),
    );

    $form['widget']['widget_settings']['links'][$this->id]['hide_facet_count'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide the facet count'),
      '#default_value' => !empty($this->settings->settings['hide_facet_count']) ? $this->settings->settings['hide_facet_count'] : FALSE,
      '#states' => $states,
    );
  }

}

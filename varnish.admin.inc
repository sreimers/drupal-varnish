<?php
// $Id:

/**
 * @file varnish.admin.inc
 * Administrative functions for Varnish integration.
 */
 
 
/**
* Menu callback for varnish admin settings.
*/
function varnish_admin_settings_form() {
  $form = array();
  
  // Decide whether or not to flush caches on cron runs.
  $form['varnish_flush_cron'] = array(
    '#type' => 'radios',
    '#title' => t('Flush page cache on cron?'),
    '#options' => array(
      0 => t('Disabled'), 
      1 => t('Enabled (with respect for cache_lifetime)'),
    ),
    '#default_value' => variable_get('varnish_flush_cron', 0),
    '#description' => t('Internally Drupal will attempt to flush its page cache every time cron.php runs. This can mean too-frequent cache flushes if you have cron running frequently. NOTE: this cache flush is global!'),
  );
  
  if (!extension_loaded('sockets')) {
    drupal_set_message(t('<a href="http://us3.php.net/manual/en/sockets.installation.php">PHP Sockets extension</a> not enabled. Varnish terminal communication configuration skipped.'), 'error');
    return system_settings_form($form);
  }
  
  // Begin socket-dependent configuration.
  
  $form['varnish_control_terminal_server'] = array(
    '#type' => 'textfield',
    '#title' => t('Varnish Control Terminal Server'),
    '#default_value' => variable_get('varnish_control_terminal_server', '127.0.0.1'),
    '#required' => TRUE,    
    '#description' => t('Set this to the server IP or hostname that varnish runs on. This must be configured for Drupal to talk to Varnish.'),
  );
  $form['varnish_control_terminal_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Varnish Control Terminal Server'),
    '#default_value' => variable_get('varnish_control_terminal_port', '6082'),
    '#required' => TRUE,
    '#description' => t('Set this to the port on which your varnish control terminal runs. This must be configured for Drupal to talk to Varnish.'),
  );
  $form['varnish_control_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Varnish Control Key'),
    '#default_value' => variable_get('varnish_control_key', ''),
    '#description' => t('Optional: if you have established a secret key for control terminal access, please put it here.'),
  );
  $form['varnish_cache_clear'] = array(
    '#type' => 'radios',
    '#title' => t('Varnish Cache Clearing'),
    '#options' => array(
      VARNISH_DEFAULT_CLEAR => t('Drupal Default'), 
      VARNISH_NO_CLEAR => t('None'),
    ),
    '#default_value' => variable_get('varnish_cache_clear', VARNISH_DEFAULT_CLEAR),
    '#description' => t('What kind of cache clearing Varnish should utilize. Drupal default will clear all page caches on node updates and cache flush events. None will allow pages to persist for their full max-age; use this if you want to write your own cache-clearing logic.'),
  );
  
  // Detect expire module and add this option.
  if (module_exists('expire')) {
    $form['varnish_cache_clear']['#options'][VARNISH_SELECTIVE_CLEAR] = t('Selective (experimental; uses expire.module)');
    $form['varnish_cache_clear']['#description'] .= ' '. t('Selective will clear a list of URLs you specify as well as any node urls themselves.');
  }
  else {
    $form['varnish_cache_clear']['#description'] .= ' '. t('Installing the !link will enable "Selective" clearing.', array('!link' => '<a href="http://drupal.org/project/expire" target="_blank">'. t('Expire module') .'</a>'));
  }
  
  $form['varnish_stats'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Stats'),
  );
  if ($_GET['stats'] == 1) {  
    $status = _varnish_terminal_run('stats', 50000);
    $form['varnish_stats']['#collapsed'] = FALSE;
  }
  else {
    $status = l(t('Fetch stats'), 'admin/settings/varnish', array('query' => 'stats=1'));
    $form['varnish_stats']['#collapsed'] = TRUE;
  }
  $form['varnish_stats']['data'] = array(
    '#type' => 'markup',
    '#prefix' => '<pre>',
    '#suffic' => '</pre>',
    '#value' => $status,
  );
  
  return system_settings_form($form);
}


/**
* Menu callback for varnish admin settings.
*/
function varnish_admin_reports_page() {
  // connect to varnish and do a full status report
  $status = _varnish_terminal_run('stats', 50000);
  return "<pre>$status</pre>";
}
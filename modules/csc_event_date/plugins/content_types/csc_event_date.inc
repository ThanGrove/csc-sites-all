<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('CSC Event Date'),
  'description' => t('Panel to hide a date\'s time when node\'s tba field is checked'),
  'category' => t('CSC Plugins'),  
	'edit form' => 'csc_event_date_edit_form',
  'render callback' => 'csc_event_date_render',
  'admin info' => 'csc_event_date_admin_info',
);

/**
 * 'admin info' callback for panel pane.
 */
function csc_event_date_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass;
    $block->title = 'CSC Event Date';
    $block->content = 'Show CSC Event\'s date';
    return $block;
  }
}


function csc_event_date_render($subtype, $conf, $panel_args, $context = NULL) {
  $nodeid = 0;
  if (arg(0) == 'node' && is_numeric(arg(1))) $nodeid = arg(1);
	if (empty($nodeid) && count($panel_args) > 0) { $nodeid = $panel_args[0]; }
  if ($nodeid){
    $node = node_load($nodeid);
    $lng = $node->language;

    if(!empty($node->field_event_date)) {
    	$tba = (!empty($node->field_tba)) ? $node->field_tba['und'][0]['value'] : 0;
    	$format_type = ($tba == 1)?'panopoly_day':'full';
			$tbastr = ($tba == 1)? ', Time TBA': '';
			$display = array('settings' => array('format_type' => $format_type));
			$dout = field_view_field('node', $node, 'field_event_date', $display);
      

      $output = '<div id="csc-event-date">';
      foreach($dout['#items'] as $n => $item) {
      	$output .= $dout[$n]['#markup'] . $tbastr;
      }
      $output.= "</div>\n";

      $block = new stdClass();
      $block->title = '';
      $block->content = $output;

      return $block;
    }
  }

}


/**
 * 'Edit form' callback for the content type.
 */
function csc_event_date_edit_form($form, &$form_state) {
  // No settings beyond context, which has already been handled.
  return $form;
}
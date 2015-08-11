<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('CSC Class Date'),
  'description' => t('Panel to show a class\'s dates'),
  'category' => t('CSC Plugins'),  
	'edit form' => 'csc_class_date_edit_form',
  'render callback' => 'csc_class_date_render',
  'admin info' => 'csc_class_date_admin_info',
);

/**
 * 'admin info' callback for panel pane.
 */
function csc_class_date_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass;
    $block->title = 'CSC Class Date';
    $block->content = 'Show CSC Class\'s date';
    return $block;
  }
}


function csc_class_date_render($subtype, $conf, $panel_args, $context = NULL) {
  $nodeid = 0;
  if (arg(0) == 'node' && is_numeric(arg(1))) $nodeid = arg(1);
	if (empty($nodeid) && count($panel_args) > 0) { $nodeid = $panel_args[0]; }
  if ($nodeid){
    $node = node_load($nodeid);
    $lng = $node->language;
		$ew = entity_metadata_wrapper('node', $node);
		$hd = $ew->field_hide_date->value();
    if(!$hd && !empty($node->field_date)) {
    	
			$dout = field_view_field('node', $node, 'field_date');
    	$tba = (empty($node->field_tba) || $node->field_tba['und'][0]['value'] == 0) ?  FALSE : TRUE;
    	$online = (empty($node->field_online_course) || $node->field_online_course['und'][0]['value'] == 0) ?  FALSE : TRUE;
			if($tba || $online) {
				$display = array('settings' => array('format_type' => 'panopoly_day'));
				$dout = field_view_field('node', $node, 'field_date', $display);
			}
      $output = '<div id="csc-class-date">';
			
			// If there's a repeat rule, custom format the string for presentation in block
			if(!empty($dout['#items'][0]['rrule'])) {
				$item = $dout['#items'][0];
				$v1 = (isset($item['value'])) ? $item['value'] : '';
				$v2 = (isset($item['value2'])) ? $item['value2'] : '';
				if($rrstr = csc_class_date_repeat_rule_render($item['rrule'], $v1,  $v2)) {
					$output .= '<p>' . $rrstr . '</p>';
				} else {
					$output .= $dout[0]['#markup']; // if rule can't be rendered use default markup
				}
			} else {
			// Otherwise use pre-rendered markup for each item (should only be one)
	      foreach($dout['#items'] as $n => $item) {
	      	$output .= $dout[$n]['#markup'];
	      }
				if ($online) { $output = str_replace('class="date-display-single">', 'class="date-display-single">Beginning ', $output); }
				else if($tba) { $output .= ', Time TBA'; }
			}
			
      $output.= "</div>\n";

      $block = new stdClass();
      $block->title = '';
      $block->content = $output;
      return $block;
    }
  }

}

function csc_class_date_repeat_rule_render($rule, $t1='', $t2='') {
	$rrule = array();
  $rules = explode(';', str_replace('RRULE:', '', $rule));
  foreach($rules as $rule) {
    $parts = explode('=', $rule);
    $key = $parts[0];
    $value = $parts[1];
    $rrule[$key] = $value;
  }
	$outstr = FALSE;
	// Weekly by day
	if($rrule['FREQ'] == 'WEEKLY' && isset($rrule['BYDAY'])) {
		$days = date_repeat_dow_day_options();
		$prefix = '';
		if($rrule['INTERVAL'] == 2) { $prefix = t('other') . ' '; }
		if($rrule['INTERVAL'] == 3) { $prefix = t('third') . ' '; }
		if (isset($rrule['UNTIL'])) {
			$enddate = date('D. M jS, Y', strtotime($rrule['UNTIL']));
			$weeks = ceil((strtotime($rrule['UNTIL'], 0) - strtotime($t1, 0)) / 604800);
			$pl = ($weeks > 1) ? 's' : '';
			$enddate .= " ($weeks week{$pl})";
		} else if (isset($rrule['COUNT'])) {
			$enddate = date('D. M jS, Y', strtotime($t1 . " + {$rrule['COUNT']} weeks")) . " ({$rrule['COUNT']} weeks)";
		}
		$dayst = $days[$rrule['BYDAY']];
		if( strpos($rrule['BYDAY'], ",") > -1) {
			$dpts = explode(',', $rrule['BYDAY']);
			foreach($dpts as $d1) {
				$dayst .= $days[$d1] . ', ';
			}
			$dayst = substr($dayst, 0, strlen($dayst) - 2);
		}
		if(empty($t1)) {
			$outstr = t('Every @int@day until @enddate', array(
										'@int' => $prefix, 
										'@day' => $dayst, 
										'@enddate' => $enddate,
								));
		} elseif (empty($t2)) {
			$outstr = t('Every @int@day from @starttime until @enddate', array(
										'@int' => $prefix, 
										'@day' => $dayst, 
										'@starttime' => csc_class_date_parse_time($t1),
										'@enddate' => $enddate,
								));
		} else {
			$outstr = t("@startdate - @enddate<br/>Every @int@day from @starttime to @endtime", array(
										'@int' => $prefix, 
										'@day' => $dayst, 
										'@starttime' => csc_class_date_parse_time($t1),
										'@startdate' => csc_class_date_parse_time($t1, 'date'),
										'@endtime' => csc_class_date_parse_time($t2),
										'@enddate' => $enddate,
								));
		}
		
	} else if ($rrule['FREQ'] == 'DAILY') { // Daily frequency
		if (isset($rrule['UNTIL'])) {
			$enddate = date('D. M jS, Y', strtotime($rrule['UNTIL']));
			$weeks = ceil((strtotime($rrule['UNTIL'], 0) - strtotime($t1, 0)) / 604800);
			$pl = ($weeks > 1) ? 's' : '';
			$enddate .= " ($weeks week{$pl})";
		} else if (isset($rrule['COUNT'])) {
			$enddate = date('D. M jS, Y', strtotime($t1 . " + {$rrule['COUNT']} weeks")) . " ({$rrule['COUNT']} weeks)";
		}
		$outstr = t("@startdate - @enddate<br/>Monday through Friday, @starttime to @endtime", array(
										'@starttime' => csc_class_date_parse_time($t1),
										'@startdate' => csc_class_date_parse_time($t1, 'date'),
										'@endtime' => csc_class_date_parse_time($t2),
										'@enddate' => $enddate,
								));
	} else {
		// Add Other Rules here as needed for repeat rule render
	} 
	return $outstr;
}

function csc_class_date_parse_time($d, $type='time') {
	// Convert Greenwich time to EST when Formatting date
	// TODO: May have to remove or change when implementing user timezones or other timezone fixes
	if ($type == 'time' ) {
		$est = date_default_timezone_get();
		$gmt = 'Europe/London';
		date_default_timezone_set($gmt);
		$fd = format_date(strtotime($d), 'custom', 'g:i A', $est);
		date_default_timezone_set($est);
		// Hack to account for Daylight savings time
		$timestamp = strtotime($d);
	 	$dls_start = strtotime("Second Sunday March ". date('Y', $timestamp));
	  $dls_end = strtotime("First Sunday November ". date('Y', $timestamp));
		$is_dlst = ($timestamp >= $dls_start && $timestamp <= $dls_end)  ? TRUE : FALSE;
		if ($is_dlst) {
			$pts = explode(':', $fd);
			$pts[0] = ($pts[0] + 1) % 12;
			$fd = implode(':', $pts);
		}
		return $fd;
	} else {
		$fd = format_date(strtotime($d), 'custom', 'D. M j, Y');
		return $fd;
	}
}

/**
 * 'Edit form' callback for the content type.
 */
function csc_class_date_edit_form($form, &$form_state) {
  // No settings beyond context, which has already been handled.
  return $form;
}
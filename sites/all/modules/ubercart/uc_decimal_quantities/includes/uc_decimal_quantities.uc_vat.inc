<?php
function uc_decimal_quantities_vat_cart_review_table($show_subtotal = TRUE) {
  $tax_name = variable_get('uc_vat_name', 'VAT');

  $subtotal = 0;
  $subtotal_tax = 0;
  $subtotal_nontax = 0;

  // Set up table header.
  $header = array();
  $header[] = array('data' => t('Qty'), 'class' => 'qty');
  $header[] = array('data' => t('Products'), 'class' => 'products');
  if (variable_get('uc_vat_hide_checkout_exclusive', FALSE)) {
    $header[] = array('data' => t('incl. @tax', array('@tax' => $tax_name)), 'class' => 'price vat', 'style' => 'white-space: nowrap;');
  }
  else {
    $header[] = array('data' => t('Price excl. @tax', array('@tax' => variable_get('uc_vat_name', 'VAT'))), 'class' => 'price excl-vat', 'style' => 'white-space: nowrap;');
    $header[] = array('data' => $tax_name, 'class' => 'price vat');
  }
  $header[] = array('data' => t('Total'), 'class' => 'price');

  $context = array(
    'location' => 'cart-checkout-item',
  );

  // Set up table rows.
  $contents = uc_cart_get_contents();
  foreach ($contents as $item) {
    $node = node_load($item->nid);
    $price_info = array(
      'price' => $item->price,
      'qty' => $item->qty,
    );
    $context = array(
      'type' => 'cart_item',
      'revision' => 'altered',
      'subject' => array(
        'cart' => $contents,
        'cart_item' => $item,
        'node' => $node,
      ),
    );

    $total = uc_price($price_info, $context);
    $tax_total = 0;
    foreach (uc_taxes_rate_load() as $tax) {
      if (in_array($node->type, $tax->taxed_product_types) && ($tax->shippable == 0 || $node->shippable == 1)) {
        $tax_total += $total - $total / (1 + $tax->rate);
      }
    }
    $price = $total - $tax_total;

    $subtotal_nontax += $price;
    $subtotal_tax += $tax_total;
    $subtotal += $total;

    $description = check_plain($item->title);
    foreach (module_implements('cart_item_description') as $module) {
      $description .= module_invoke($module, 'cart_item_description', $item);
    }

    // Remove node from context to prevent the price from being altered.
    unset($context['subject']);
    $context['type'] = 'amount';
    $context['revision'] = 'themed-original';

    if (_product_allows_decimal_quantities($item)) {
      $quantity_cell = array(
        'data' => t('@qty@unit', array(
          '@qty' => number_format($item->qty / UC_DECIMAL_QUANTITIES_FACTOR, 2),
          '@unit' => UC_DECIMAL_QUANTITIES_UNIT,
        )),
        'class' => 'qty',
      );
    }
    else {
      $quantity_cell = array(
        'data' => t('@qty&times;', array(
          '@qty' => $item->qty,
        )),
        'class' => 'qty'
      );
    }

    $row = array();
    $row[] = $quantity_cell;
    $row[] = array('data' => $description, 'class' => 'products');

    $vat_array = array();
    if (!variable_get('uc_vat_hide_checkout_exclusive', FALSE)) {
      $row[] = array('data' => uc_price($price, $context), 'class' => 'price excl-vat');
    }
    $row[] = array('data' => uc_price($tax_total, $context), 'class' => 'price vat');
    $row[] = array('data' => uc_price($total, $context), 'class' => 'price');
    $rows[] = $row;
  }

  // Add the subtotal as the final row.
  if ($show_subtotal) {
    $context = array(
      'type' => 'amount',
      'revision' => 'themed-original',
    );
    $row = array('class' => 'subtotal', 'data' => array());
    if (!variable_get('uc_vat_hide_checkout_exclusive', FALSE)) {
      $row['data'][] = array('data' => '<span id="subtotal-title">' . t('Subtotals:') . '</span> ' . uc_price($subtotal_nontax, $context), 'colspan' => 3, 'class' => 'subtotal');
    }
    else {
      $row['data'][] = array('data' => '<span id="subtotal-title">' . t('Subtotals:') . '</span>', 'colspan' => 2, 'class' => 'subtotal');
    }
    $row['data'][] = array('data' => uc_price($subtotal_tax, $context), 'class' => 'price vat');
    $row['data'][] = array('data' => uc_price($subtotal, $context));
    $rows[] = $row;
  }

  return theme('table', $header, $rows, array('class' => 'cart-review'));
}
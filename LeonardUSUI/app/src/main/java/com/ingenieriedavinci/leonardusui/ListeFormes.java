package com.ingenieriedavinci.leonardusui;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

public class ListeFormes extends AppCompatActivity
{
    //Statics - Types de formes
    private static final int POLYGONES = 0;
    private static final int ELLIPSES = 1;
    private static final int SPIRALES = 2;
    private static final int PARALLELOGRAMMES = 3;
    private static final int EMOJI = 4;
    private static final int AUTRES = 5;

    //Variables
    int typeFormes;

    Forme[] formes;
    ArrayAdapter<Forme> adapteurFormes;

    //Constructeurs
    public ListeFormes() {}

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_liste_formes);

        //Attributions
        typeFormes = getIntent().getExtras().getInt("typeFormes");

        switch(typeFormes)
        {
            case POLYGONES:
                formes = new Forme[11];//De 2 à 12 inclu
                formes[0] = new Forme("Digone","forme_polygone_digone");
                formes[1] = new Forme("Triangle","forme_polygone_triangle");
                formes[2] = new Forme("Carré","forme_polygone_carre");
                formes[3] = new Forme("Pentagone","forme_polygone_pentagone");
                formes[4] = new Forme("Hexagone","forme_polygone_hexagone");
                formes[5] = new Forme("Heptagone","forme_polygone_heptagone");
                formes[6] = new Forme("Octogone","forme_polygone_octogone");
                formes[7] = new Forme("Ennéagone","forme_polygone_enneagone");
                formes[8] = new Forme("Décagone","forme_polygone_decagone");
                formes[9] = new Forme("Hendécagone","forme_polygone_hendecagone");
                formes[10] = new Forme("Dodécagone","forme_polygone_dodecagone");

                break;
            case ELLIPSES:
                formes = new Forme[1];
                formes[0] = new Forme("Cercle", "");

                break;
            case SPIRALES:
                formes = new Forme[1];
                formes[0] = new Forme("À venir", "");

                break;
            case PARALLELOGRAMMES:
                formes = new Forme[1];
                formes[0] = new Forme("À venir", "");

                break;
            case EMOJI:
                formes = new Forme[3];
                formes[0] = new Forme("Content", "");
                formes[1] = new Forme("Triste", "");
                formes[2] = new Forme("Blazé", "");

                break;
            case AUTRES:
                formes = new Forme[2];
                formes[0] = new Forme("Informatique", "forme_autre_informatique");
                formes[1] = new Forme("Électrique", "forme_autre_electrique");

                break;
            default:
                break;
        }

        adapteurFormes = new AdapteurArrayListeFormes(this, 0, formes);
        final ListView listeFormes = (ListView) findViewById(R.id.listeFormes);
        listeFormes.setAdapter(adapteurFormes);

        listeFormes.setOnItemClickListener(new AdapterView.OnItemClickListener()
        {
            public void onItemClick(AdapterView<?> parent, View view, int position, long id)
            {
                //TODO
                /*partieActive.getJoueurs().get(position).setCompteBancaire(partieActive.getJoueurs().get(position));
                adapteur.notifyDataSetChanged();
                BanqueZirconienne.actualisationComptesBancaires();
                finish();*/
            }
        });


    }

    //Méthodes
    public void startActivity(int typeFormes, Context context)
    {
        Intent intent = new Intent(context, ListeFormes.class);
        intent.putExtra("typeFormes", typeFormes);
        context.startActivity(intent);
    }

    //custom ArrayAdapters
    class AdapteurArrayListeFormes extends ArrayAdapter<Forme>
    {

        private Context context;
        private Forme[] formes;

        public AdapteurArrayListeFormes(Context context, int resource, Forme[] objects)
        {
            super(context, resource, objects);

            this.context = context;
            this.formes = objects;
        }

        public View getView(int position, View convertView, ViewGroup parent)
        {
            String nomForme = formes[position].getImageDrawableString();

            //View d'un placement
            LayoutInflater inflater = (LayoutInflater) context.getSystemService(Activity.LAYOUT_INFLATER_SERVICE);
            View view = inflater.inflate(R.layout.layout_bouton_forme, null);
            view.setPaddingRelative(20,20,20,20);

            TextView nom = (TextView) view.findViewById(R.id.typeForme);
            ImageViewCarre imageForme = view.findViewById(R.id.imageForme);

            nom.setText(formes[position].getNom());
            Bitmap bitmap = BitmapFactory.decodeResource(context.getResources(), getResources().getIdentifier(nomForme, "drawable", getContext().getPackageName()));
            bitmap = getBitmapGrandeur(bitmap, 180);
            imageForme.setImageBitmap(bitmap);

            return view;
        }

        //Méthodes
        public Bitmap getBitmapGrandeur(Bitmap image, int maxSize)
        {
            int width, height;
            if(image != null)
            {
                width = image.getWidth();
                height = image.getHeight();

                float bitmapRatio = (float) width / (float) height;
                if (bitmapRatio > 1)
                {
                    width = maxSize;
                    height = (int) (width / bitmapRatio);
                } else
                {
                    height = maxSize;
                    width = (int) (height * bitmapRatio);
                }
                return Bitmap.createScaledBitmap(image, width, height, true);
            }
            else
            {
                return image;
            }
        }
    }
}


/*

*/